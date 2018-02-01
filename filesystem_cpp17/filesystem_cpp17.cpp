#include <locale>
#include <iostream>
#include <fstream>
using namespace std;
#include <filesystem>
using namespace std::experimental::filesystem;

int main() {
	path current = L".";
	cout << "Current folder: " << current << endl;
	cout << "...absolute: " << absolute(current) << endl;	// starts at root (may have . or .. in the path)
	cout << "...canonical: " << canonical(current) << endl;	// no . or ..
	cout << "...easier cwd: " << current_path() << endl;

	path parent = canonical("..");
	cout << "Parent folder: " << parent << endl;

	// is the folder/file there?
	if (exists(parent))
		cout << "All OK - file exists\n";

	// make a folder
	create_directory("sub");
	create_directories("sub2/sub1");

	ofstream out("sub2/test.txt");
	out << "some data";
	out.close();

	// remove a folder
	remove(path("sub")); // without 'path' the compiler calls the wrong 'remove function' (Older POSIX)
	remove_all("sub2");

	// free space
	cout.imbue(locale(""));
	cout << "Free space: " << space("c:").available << " of " << space("C:").capacity << endl;
	
	// make your own filename 
	path myfile = current_path() / "sub" / "myfile.ext";
	cout << "Myfile = " << myfile << endl;

	// scan a folder 
	cout << endl;
	void scan(path const&);
	scan("..");
	void rscan(path const&);
	rscan("..");
	void rscan2(path const&, unsigned i = 0);
	rscan2("..");
}

void scan(path const& f) {
	directory_iterator d(f);	// beginning of folder 'f'
	directory_iterator e;		// represents the end of folder
	while (d != e) {
		cout << d->path() << (is_directory(d->status()) ? " [dir]" : "") << " ext=" << d->path().extension() << endl;
		++d;
	}
}

void rscan(path const& f) {
	cout << endl;
	for (recursive_directory_iterator d(f), e; d != e; ++d) {
		cout << d->path() << (is_directory(d->status()) ? " [dir]" : "") << " ext=" << d->path().extension() << endl;
	}
}

void rscan2(path const& f, unsigned i) {
	string indent(i, ' ');
	cout << indent << "Folder = " << system_complete(f) << endl;
	for (directory_iterator d(f), e; d != e; ++d) {
		cout << indent << d->path() << (is_directory(d->status()) ? " [dir]" : "") << " ext=" << d->path().extension() << endl;

		if (is_directory(d->status()))
			rscan2(d->path, i + 1);
	}
}