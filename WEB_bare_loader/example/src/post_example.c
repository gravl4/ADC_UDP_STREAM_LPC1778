/**
 * @file
 * WEB_bare_loader
 * @Ten Vyacheslav
 * 24/01/2023
 */

#include "lwip/opt.h"

#include "httpd.h"
#include "lwip/def.h"
#include "lwip/mem.h"

#include <stdio.h>
#include <string.h>

#ifndef true
#define true ((u8_t)1)
#endif

#ifndef false
#define false ((u8_t)0)
#endif

/** define LWIP_HTTPD_EXAMPLE_GENERATEDFILES to 1 to enable this file system */
#ifndef LWIP_HTTPD_EXAMPLE_SIMPLEPOST
#define LWIP_HTTPD_EXAMPLE_SIMPLEPOST 1
#endif

#if LWIP_HTTPD_EXAMPLE_SIMPLEPOST

#if !LWIP_HTTPD_SUPPORT_POST
#error This needs LWIP_HTTPD_SUPPORT_POST
#endif

static void *current_connection;
static void *valid_connection;

#define CONTENT_LENGTH_TAG      "Content-Length:"
#define OCTET_STREAM_TAG      "application/octet-stream\r\n\r\n"
#define EMPTY_LINE_TAG          "\r\n\r\n"

#define FWUPDATE_STATUS_ERROR          -1
#define FWUPDATE_STATUS_NONE            0
#define FWUPDATE_STATUS_INPROGRESS      1
#define FWUPDATE_STATUS_DONE            2
#define CMD_SUCCESS 0

typedef enum {
    FWUPDATE_STATE_HEADER,
    FWUPDATE_STATE_OCTET_START,
    FWUPDATE_STATE_OCTET_STREAM,

} fwupdate_state_t;

typedef struct {

    fwupdate_state_t state;

    int content_length;
    int file_length;
    int accum_length;

    /* Data stream accumulation buffer to meet 8-byte size-alignment constraint with v1.3.0 .sfb file update */
    uint8_t accum_buf[8];
    uint8_t accum_buf_len;

} fwupdate_t;

static fwupdate_t fwupdate;

/**
   * @brief  Secure Engine Error definition
   */
typedef enum
{
    SE_ERROR = 0U,
    SE_SUCCESS = !SE_ERROR
} SE_ErrorStatus;

char dataArr[1024];
static int flash_pos = 0x10000;
static int curlen = 0;
static int flash_cnt = 0;
int close = true;
int reboot = false;

extern unsigned write_flash(unsigned *dst, char *src, unsigned no_of_bytes);
extern void jump_to_app_section();

static int fwupdate_multipart_state_machine(char *buf, u16_t buflen);
#define DEBUGOUT(...) printf(__VA_ARGS__)


err_t httpd_post_begin(void *connection, const char *uri,
		const char *http_request, u16_t http_request_len, int content_len,
		char *response_uri, u16_t response_uri_len, u8_t *post_auto_wnd) {
	LWIP_UNUSED_ARG(connection);
	LWIP_UNUSED_ARG(http_request);
	LWIP_UNUSED_ARG(http_request_len);
	LWIP_UNUSED_ARG(content_len);
	LWIP_UNUSED_ARG(post_auto_wnd);
	if (!memcmp(uri, "/upload", 7)) {
		if (current_connection != connection) {

//		int ret = fwupdate_multipart_state_machine(http_request, http_request_len);
			fwupdate.state = FWUPDATE_STATE_OCTET_START;
			fwupdate.content_length = content_len;

			current_connection = connection;
			valid_connection = NULL;
			/* default page is "login failed" */
			snprintf(response_uri, response_uri_len, "/loginfail.html");
			/* e.g. for large uploads to slow flash over a fast connection, you should
			 manually update the rx window. That way, a sender can only send a full
			 tcp window at a time. If this is required, set 'post_aut_wnd' to 0.
			 We do not need to throttle upload speed here, so: */
			*post_auto_wnd = 1;
			return ERR_OK;
		}
	}
	return ERR_VAL;
}

err_t httpd_post_receive_data(void *connection, struct pbuf *p) {
	err_t ret = ERR_OK;
	LWIP_ASSERT("NULL pbuf", p != NULL);

	if (current_connection == connection) {
		int ret = fwupdate_multipart_state_machine((u8_t*) p->payload, p->len);
		if (ret == FWUPDATE_STATUS_NONE) {
			/* ignore */
		} else if (ret == FWUPDATE_STATUS_INPROGRESS) {
			/* Don't close the connection! */
			close = false;
		} else {
			/* Some result, we should close the connection now. */
			close = true;

			if (ret == FWUPDATE_STATUS_DONE) {
				/* reboot after we close the connection. */
				reboot = true;
			}
		}

		/* not returning ERR_OK aborts the connection, so return ERR_OK unless the
		 connection is unknown */
		ret = ERR_OK;
	} else {
		ret = ERR_VAL;
	}
	/* this function must ALWAYS free the pbuf it is passed or it will leak memory */
	pbuf_free(p);

	return ret;
}

void httpd_post_finished(void *connection, char *response_uri, u16_t response_uri_len) {
	if (reboot) {
		DEBUGOUT("Rebooting\n");
		/* Wait 5 seconds. */
		//vTaskDelay(5000);
/*		fwupdate_send_success(conn,
				"<html><head><meta http-equiv=\"refresh\" content=\"15; url='/firmwareupdate.html'\" /></head>"
				"<body>Firmware upload success! Rebooting and applying update in 5 seconds. Auto reload in 15.</body></html>");
*/
		jump_to_app_section();
	}

	/* default page is "login failed" */
	snprintf(response_uri, response_uri_len, "/loginfail.html");
	if (current_connection == connection) {
		if (valid_connection == connection) {
			/* login succeeded */
			snprintf(response_uri, response_uri_len, "/session.html");
		}
		current_connection = NULL;
		valid_connection = NULL;
	}
}

/* Function to extract version information from URI */
/*static uint32_t get_version(const char *vstr)
{
	int major = 0, minor = 0;
	sscanf(vstr, "HTTP/%d.%d", &major, &minor);
	return (major << 16) | minor;
}*/

static const char* fwupdate_state_str(fwupdate_state_t state)
{
    switch (state) {
    case FWUPDATE_STATE_HEADER: return "HEADER";
    case FWUPDATE_STATE_OCTET_START: return "OCTET_START";
    case FWUPDATE_STATE_OCTET_STREAM: return "OCTET_STREAM";
    default:
        return "<unknown>";
    }
}

/*static void fwupdate_send_success(struct netconn* conn, const char* str)
{
//    netconn_write(conn, (const unsigned char*)fwupdate_success_html, (size_t)sizeof(fwupdate_success_html), NETCONN_NOCOPY);
//    netconn_write(conn, (const unsigned char*)str, (size_t)strlen(str), NETCONN_NOCOPY);
}*/

static int fwupdate_multipart_state_machine(char *buf, u16_t buflen) {
	/* So how the multipart file upload appears to work (from Chrome):
	 *  1. Get the POST header in one netconn_recv() buffer.
	 *  2. From header get the content-length which will be the number of
	 *     bytes in following netconn_recv() buffer(s).
	 *  3. Compute number of file bytes by subtracting content of second netconn_recv()  buffer
	 *     up to the "Content-Type: application/octet-stream\r\n\r\n"
	 *  4. Continue to process netconn_recv()  buffers until file bytes are received.
	 *  5. Send "200 OK" response and close connection.
	 */

	/* Process the buffer as long as we can in one of these two scenarios only:
	 In case of Chrome, multipart octet-stream comes in separate packet/buffer while
	 in case of Postman, multipart octet-stream comes in same packet/buffer as POST header.
	 */

	int ret = FWUPDATE_STATUS_NONE;

	char *buf_start = buf;
	char *buf_end = buf + buflen; /* points to byte AFTER end of buffer! */

	/* Diagnostics */
	//print_buf((uint8_t*)buf, buflen);
	/* We advance the buffer pointer as parts are consumed.  Keep processing
	 until buffer pointer gets nulled either as a result of not finding what we want
	 or explicitly on error condition.
	 */
	while (buf && buf < buf_end) {

		DEBUGOUT("@ fwupdate buf_start=%p, buf=%p buf_end=%p state=%s\n",
				buf_start, buf, buf_end, fwupdate_state_str(fwupdate.state));

		switch (fwupdate.state) {
		case FWUPDATE_STATE_HEADER:
			/* Look for POST header */
			if ((buflen >= 12) && (strncmp(buf, "POST /upload", 12) == 0)) {
				ret = FWUPDATE_STATUS_ERROR; /* Error until we go to in progress */
				/* This buffer must have the complete POST header */
				DEBUGOUT("@ fwupdate -     Scanning HEADER\n");
				/* Diagnostics */
//				print_buf((uint8_t*) buf, buflen);
				buf = strstr(buf, CONTENT_LENGTH_TAG);
				if (buf) {
					buf += strlen(CONTENT_LENGTH_TAG);
					fwupdate.content_length = atoi(buf);
					/* sanity check  */
					if (fwupdate.content_length > 0) {
						/* Having obtained content length, need to next find the
						 end of the header, denoted by an empty line.
						 This is where the content-length starts from. */
						buf = strstr(buf, EMPTY_LINE_TAG);
						if (buf) {
							buf += strlen(EMPTY_LINE_TAG);
							/* To get the length of the file, have to subtract all bytes up to and including
							 application/octet-stream\r\n\r\n"
							 */
							/* advance the buffer reference point to here */
							buf_start = buf;
							/* Got what we need/expect and must wait for the "octet-stream" content-type in
							 the next multipart header packet. */
							fwupdate.state = FWUPDATE_STATE_OCTET_START;
							ret = FWUPDATE_STATUS_INPROGRESS;

							DEBUGOUT("@ fwupdate - Have content len=%d\n", fwupdate.content_length);
						}
					} else {
						buf = 0;
					}
				}
			} else {
				buf = 0;
			}
			break;
		case FWUPDATE_STATE_OCTET_START:
			DEBUGOUT("FWUPDATE_STATE_OCTET_START\n");
			ret = FWUPDATE_STATUS_ERROR;
			fwupdate.state = FWUPDATE_STATE_HEADER;

			/* Look for the start of the firmware update file */
			buf = strstr(buf, OCTET_STREAM_TAG);
			if (buf) {
				buf += strlen(OCTET_STREAM_TAG);
				int multipart_length = buf - buf_start;
				/* Strictly speaking this computed file length is incorrect - it is a bit too large as it includes
				 the ending multipart boundary tag of few dozen chars. This doesn't impact the update process because the
				 patching engine manages byte counters internally. This is just for diagnostic purposes and sanity check.
				 */
				fwupdate.file_length = fwupdate.content_length
						- multipart_length;
				DEBUGOUT("@ fwupdate content len=%d multipart len=%d file len=%d\n",
						fwupdate.content_length, multipart_length,
						fwupdate.file_length);
				/* Now can init the update */
//				SE_PATCH_StartInfo info = { .type = SE_PATCH_ImageType_APP, /* App type */
//				.rebootDelay = SE_PATCH_RebootDelay_NEXT, /* Allow us to to control when to reboot so we can send out a confirmation response to the web client. */
				//.rebootDelay = SE_PATCH_RebootDelay_IMMEDIATE, /* Patch engine automatically reboots when update is complete (cannot send a response to web client) */
//						.totalLength = fwupdate.file_length, /* Could be a patch or full image; either way used as a sanity check but not necessary to use here. */
//				};
//				SE_PATCH_Status se_patch_status;
//				SE_PATCH_InitStatus(&se_patch_status);
//				SE_ErrorStatus se_status = SE_PATCH_Init(&se_patch_status, &info);
//				printf("@ fwupdate INIT %d\n", se_status);
//				fwupdate_PrintStatus(&se_patch_status);
//				if (se_status == SE_SUCCESS)
				{
					/* Now need to consume rest of buffer as the file */
					fwupdate.state = FWUPDATE_STATE_OCTET_STREAM;
					fwupdate.accum_length = 0;
					fwupdate.accum_buf_len = 0;
					ret = FWUPDATE_STATUS_INPROGRESS;
//				} else {
//					fwupdate_send_err(conn, fwupdate_GetStatusCodeString(se_patch_status.code));
				}
			}

			break;

		case FWUPDATE_STATE_OCTET_STREAM: {
			DEBUGOUT("FWUPDATE_STATE_OCTET_STREAM\n");
			/* All of "buf" until "buf_end"-1 is considered file data at this point */
			/* It is safe to feed this buffer right into the patching engine.
			 It could block here while flash is being erased & written if we have enough and correct bytes */

			/* stm32-secure-patching-bootloader v1.3.0 errata:
			 [2] SE_PATCH_Data API will fail if presented with a full-image update file (.sfb) data buffer length
			 that is not a multiple of the platform's flash write size: typically 8 bytes (doubleword).
			 Workaround is to ensure that the API is called with an 8-byte-multiple length buffer.
			 Note: this is not necessary for .sfbp (patch) files, but having it in place won't affect .sfbp updates.
			 For ethernet we have no control over how many bytes may be sent so we implement a buffer
			 scheme to accumulate bytes until we meet the criteria for each SE_PATCH_Data() call.
			 */

			/* total bytes we have in this current buffer - these all have to be processed with SE_PATCH_Data()
			 or stored in the accum_buf */
			int len = buf_end - buf;
			DEBUGOUT("@ fwupdate len=%d\n", len);
curlen += len;

			for (int i = 0; i < len; i++) {
				dataArr[flash_cnt] = buf[i];
				flash_cnt++;
				if (flash_cnt >= 1024) {
					// write
					flash_cnt = 0;
					if (write_flash(flash_pos, &dataArr[0], 1024) == CMD_SUCCESS)
					{
						flash_pos += 1024;
						DEBUGOUT("flash_pos=%d\n", flash_pos);
						memset(dataArr, 0xff, 1024);
					//} else {
						//error
					}
				}
			}

			ret = FWUPDATE_STATUS_INPROGRESS;

			/* In any case we have consumed all bytes remaining in this buffer */
			buf = 0;

			if (ret == FWUPDATE_STATUS_INPROGRESS) {
				if (curlen >= fwupdate.file_length) {
					if (flash_cnt > 0) {
						// write
						//flash_cnt = 0;
						if (write_flash(flash_pos, &dataArr[0], 1024) == CMD_SUCCESS)
						{
							//flash_pos += 1024;
							//memset(dataArr, 0xff, 1024);
						//} else {
							//error
						}
					}
					DEBUGOUT("flash finished!");

					/*fwupdate_send_success(conn,
							"<html><head><meta http-equiv=\"refresh\" content=\"15; url='/firmwareupdate.html'\" /></head>"
							"<body>Firmware upload success! Rebooting and applying update in 5 seconds. Auto reload in 15.</body></html>");
					*/
					ret = FWUPDATE_STATUS_DONE;
					fwupdate.state = FWUPDATE_STATE_HEADER;
				}
			}

			break;
		}
		}
	} /* while (buf) */

	return ret;
}

#endif /* LWIP_HTTPD_EXAMPLE_SIMPLEPOST*/
