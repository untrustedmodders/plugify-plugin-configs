#include "plugin.hpp"
#include <plugify_configs_export.h>

void ConfgisPlugin::OnPluginStart()
{
}

ConfgisPlugin g_configsPlugin;
EXPOSE_PLUGIN(PLUGIFY_CONFIGS_API, &g_configsPlugin)
