#include "plugin.hpp"
#include "config.hpp"
#include <string_view>
#include <memory>

namespace pcf
{
	extern ConfigsPlugin plugin;
}

std::unique_ptr<pcf::Config> ReadConfig(std::string_view path)
{
	return pcf::plugin.ReadConfig(path);
}

std::unique_ptr<pcf::Config> ReadConfigs(const plg::vector<std::string_view>& paths)
{
	return pcf::plugin.ReadConfigs(paths);
}

std::unique_ptr<pcf::Config> MakeConfig()
{
	return pcf::Config::Detail::MakeConfig();
}

void SetError(std::string_view error)
{
	pcf::plugin.SetError(error);
}

plg::string GetError()
{
	return pcf::plugin.GetError();
}
