#include "Dialogs/FileDialog/Location.hpp"
#include "Dialogs/FileDialog/FileFunctions.hpp"


Location::Location(std::filesystem::path path, int depth) {
	_path = getPath(path.wstring());
	_depth = depth;
	_isOpen = false;

	if (std::filesystem::is_directory(_path) || isDrive(_path.wstring()))
		_hasChildren = hasChildren(_path);
	else
		_hasChildren = false;
}

Location::~Location() {

}

void Location::open() {

	if (!_hasChildren)
		return;

	_isOpen = true;

	if (!_children.empty())
		return;

	std::error_code ec;
	auto opts = std::filesystem::directory_options::skip_permission_denied;

	for (std::filesystem::directory_iterator it(_path, opts, ec), end;
		it != end; it.increment(ec)) {

		if (ec) {
			ec.clear();
			continue;
		}

		std::filesystem::path p = getPath(it->path());

		if (p.empty())
			continue;

		if (!std::filesystem::is_directory(p))
			continue;

		auto name = p.filename().wstring();

		if (name.empty() || onlyWhitespace(name) || name == L"." || name == L"..")
			continue;

		_children.push_back(std::make_shared<Location>(p, _depth + 1));
	}
}

void Location::close() {
	_isOpen = false;
}