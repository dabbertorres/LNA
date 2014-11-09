#include "Function.hpp"

namespace lpp
{
	std::unordered_map<std::string, std::unique_ptr<BaseFunction>> BaseFunction::functions;
}
