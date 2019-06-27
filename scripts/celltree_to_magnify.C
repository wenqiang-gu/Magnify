void celltree_to_magnify(const char* filename="../data/celltree.root", const char* outname="../data/magnify.root");
void celltree_to_magnify(const char* filename, const char* outname)
{
    // gROOT->Reset();
    // gROOT->ProcessLine(".x loadClasses.C" );

	MagnifyWriter w(filename, outname, "icarus", 0);
	// MagnifyWriter w(filename, 0, "icarus");
	w.PrintInfo();
	w.Write(0); // write first event

}
