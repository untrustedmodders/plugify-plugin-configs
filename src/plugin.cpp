#include "readers/json_glaze.hpp"
#include "plugin.hpp"
#include "readerfactory.hpp"
#include <plugify-configs/plugify-configs.hpp>
#include <plugify_configs_export.h>

namespace pcf
{
	void ConfigsPlugin::OnPluginStart()
	{
		_factory = std::make_unique<ReaderFactory>();
		_factory->RegisterReader("json", &ReadJsonConfig);
	}

	void ConfigsPlugin::OnPluginEnd()
	{
		_factory.reset();
	}

	std::unique_ptr<Config> ConfigsPlugin::ReadConfig(std::string_view path)
	{
		if (!_factory) {
			return nullptr;
		}

		return _factory->ReadConfig(path);
	}

	ConfigsPlugin plugin;
}

EXPOSE_PLUGIN(PLUGIFY_CONFIGS_API, &pcf::plugin)
