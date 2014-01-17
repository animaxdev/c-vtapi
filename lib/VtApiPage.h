#ifndef VT_API_PAGE_H
#define VT_API_PAGE_H 1

#ifdef  __cplusplus

class VpPageHandler_ops;
extern "C" {
#endif

#include <stdbool.h>

#include "VtObject.h"
	
	
/**
	* Common Page Handler Header
	* This macro must be included as first member in every object,
	* that inherits this VtApiPage
	*/
#define API_OBJECT_COMMON \
VT_OBJECT_COMMON; \
struct VpPageHandler_ops *ph_ops;\
char *buffer; \
unsigned int buffer_size; \
struct VtResponse *response; \
char *api_key;



struct Session;

	
/**
* @struct VtApiPage
* @brief A generic filter object that other more specialized handler objects will inherit.
* @brief This will give us a kind of polymorphism.
*/
struct VtApiPage
{
	API_OBJECT_COMMON;

};

struct VtApiPage_ops
{
	struct VtObject_ops *obj_ops; /// Parent Ops
	
};
	
int VtApiPage_destructor(struct VtObject *obj);

struct VtApiPage* VtApiPage_alloc(struct VtApiPage_ops *ops);
struct VtApiPage* VtApiPage_new(void);

void VtApiPage_put(struct VtApiPage **);
void VtApiPage_get(struct VtApiPage *);
	
void VtApiPage_setApiKey(struct VtApiPage *api, const char *key);

size_t __VtApiPage_WriteCb( char *ptr, size_t size, size_t nmemb, void *userdata);
	
#ifdef  __cplusplus
}
#endif /*cplusplus*/

#endif
