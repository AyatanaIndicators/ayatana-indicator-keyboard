#ifndef __INDICATOR_KEYBOARD_SERVICE_H__
#define __INDICATOR_KEYBOARD_SERVICE_H__

#include <glib.h>
#include <glib-object.h>
#include "keyboard.h"

G_BEGIN_DECLS

#define INDICATOR_KEYBOARD_SERVICE(o) (G_TYPE_CHECK_INSTANCE_CAST((o), INDICATOR_TYPE_KEYBOARD_SERVICE, IndicatorKeyboardService))
#define INDICATOR_TYPE_KEYBOARD_SERVICE (indicator_keyboard_service_get_type())
#define INDICATOR_IS_KEYBOARD_SERVICE(o) (G_TYPE_CHECK_INSTANCE_TYPE((o), INDICATOR_TYPE_KEYBOARD_SERVICE))

typedef struct _IndicatorKeyboardService IndicatorKeyboardService;
typedef struct _IndicatorKeyboardServiceClass IndicatorKeyboardServiceClass;
typedef struct _IndicatorKeyboardServicePrivate IndicatorKeyboardServicePrivate;

struct _IndicatorKeyboardService
{
    GObject parent;
    IndicatorKeyboardServicePrivate *pPrivate;
};

struct _IndicatorKeyboardServiceClass
{
    GObjectClass parent_class;
    void (*pNameLost)(IndicatorKeyboardService *self);
};

GType indicator_keyboard_service_get_type(void);
IndicatorKeyboardService* indicator_keyboard_service_new();
void indicator_keyboard_service_AddKeyboardSource(IndicatorKeyboardService *pService);

G_END_DECLS

#endif /* __INDICATOR_KEYBOARD_SERVICE_H__ */
