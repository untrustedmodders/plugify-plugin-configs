#include "plugin.hpp"
#include <plugin_export.h>

void ConfgisPlugin::OnPluginStart()
{
}

ConfgisPlugin g_configsPlugin;
EXPOSE_PLUGIN(PLUGIN_API, &g_configsPlugin)
