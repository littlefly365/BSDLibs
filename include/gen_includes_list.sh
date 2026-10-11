#!/bin/sh

main()
{
	ls *.h > includes_list
	ls altq/*.h >> includes_list
	ls amd64/*.h >> includes_list
	ls arpa/*.h >> includes_list
	ls i386/*.h >> includes_list
	ls net/*.h >> includes_list
	ls netatalk/*.h >> includes_list
	ls netinet/*.h >> includes_list
	ls netinet6/*.h >> includes_list
	ls prop/*.h >> includes_list
	ls rpc/*.h >> includes_list
	ls rpcsvc/*.h >> includes_list
	ls ssp/*.h >> includes_list
	ls sys/*.h >> includes_list
	ls uvm/*.h >> includes_list
	ls x86/*.h >> includes_list
}

main
