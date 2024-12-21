#include "plugin.hpp"
#include <plugify_configs_export.h>

namespace pcf
{
	void ConfigsPlugin::OnPluginStart()
	{
	}

	void ConfigsPlugin::OnPluginEnd()
	{
	}

	ConfigsPlugin plugin;
}

EXPOSE_PLUGIN(PLUGIFY_CONFIGS_API, &pcf::plugin)
