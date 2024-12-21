#include "plugin.hpp"
#include "readerfactory.hpp"
#include <plugify-configs/plugify-configs.hpp>
#include <plugify_configs_export.h>

namespace pcf
{
	void ConfigsPlugin::OnPluginStart()
	{
		_factory = std::make_unique<ReaderFactory>();
	}

	void ConfigsPlugin::OnPluginEnd()
	{
		_factory.reset();
	}

	ConfigsPlugin plugin;
}

EXPOSE_PLUGIN(PLUGIFY_CONFIGS_API, &pcf::plugin)
