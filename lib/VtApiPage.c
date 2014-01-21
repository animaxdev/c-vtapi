#define _GNU_SOURCE


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>


#include "vtcapi_common.h"
#include "VtApiPage.h"
#include "VtResponse.h"


/**
* @name Constructor and Destructor
* @{
*/

/**
*  VtObjects constructor
*  @arg VtObject that was just allocated
*/
int VtApiPage_constructor(struct VtObject *obj)
{
	struct VtApiPage *page = (struct VtApiPage *)obj;

	DBG(DGB_LEVEL_MEM, " constructor %p\n", page);
	return 0;
}


/**
*  VtObjects destructor
*  @arg VtObject that is going to be free'd
*/
int VtApiPage_destructor(struct VtObject *obj)
{
	struct VtApiPage *page = (struct VtApiPage *)obj;

	DBG(DGB_LEVEL_MEM, " destructor %p\n", page);

	if (page->buffer)
		free(page->buffer);

	if (page->api_key)
		free(page->api_key);
    
    if (page->response) {
        VtResponse_put(&page->response);
    }

	return 0;
}

/** @} */



static struct VtObject_ops vt_page_handler_obj_ops = {
	.obj_type           = "VtApiPage",
	.obj_size           = sizeof(struct VtApiPage),
	.obj_constructor    = VtApiPage_constructor,
	.obj_destructor     = VtApiPage_destructor,
};

static struct VtApiPage_ops vt_api_ops = {
	.obj_ops                = &vt_page_handler_obj_ops, // Parent ops

};

struct VtApiPage* VtApiPage_alloc(struct VtApiPage_ops *api_ops)
{
	struct VtApiPage *api;
	api = (struct VtApiPage*) VtObject_alloc(api_ops->obj_ops);
	api->obj_ops =  api_ops->obj_ops;

	return api;
}


struct VtApiPage* VtApiPage_new(void)
{
	struct VtApiPage *VtApiPage = VtApiPage_alloc(&vt_api_ops);

	return VtApiPage;
}

/** Get a reference counter */
void VtApiPage_get(struct VtApiPage *page)
{
	VtObject_get((struct VtObject*) page);
}

/** put a reference counter */
void VtApiPage_put(struct VtApiPage **page)
{
	VtObject_put((struct VtObject**) page);
}


void VtApiPage_setApiKey(struct VtApiPage *api, const char *key)
{

	if (api->api_key)
		free(api->api_key);

	api->api_key = strdup(key);
}

void VtApiPage_resetBuffer(struct VtApiPage *api)
{
	if (api->buffer) {
		free(api->buffer);
		api->buffer = NULL;
	}
	api->buffer_size = 0;
}

size_t __VtApiPage_WriteCb( char *ptr, size_t size, size_t nmemb, void *userdata)
{
	size_t bytes = size * nmemb;  // total amount of data.
	struct VtApiPage *page_hand = (struct VtApiPage *) userdata;
	unsigned int new_buff_size = page_hand->buffer_size + bytes;

	DBG(1, "Recv %zd bytes\n", bytes);

	page_hand->buffer = realloc(page_hand->buffer, new_buff_size+1);

	if (!page_hand->buffer) {
		ERROR("Out of memory\n");
		return 0;
	}

	page_hand->buffer[new_buff_size] = 0; // null term
	memcpy(page_hand->buffer + page_hand->buffer_size, ptr, bytes);


	page_hand->buffer_size = new_buff_size;
	return bytes;
}

