//
// Distributed under the ITensor Library License, Version 1.2
//    (See accompanying LICENSE file.)
//

// input.h -- classes for reading from input files

#ifndef _ITENSOR_INPUT_H_
#define _ITENSOR_INPUT_H_
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include "itensor/types.h"
#include "itensor/util/error.h"
#include "itensor/util/optional_ptr.h"

namespace itensor {

class InputFile
    {
    public:

    InputFile(std::string fname) : filename_(fname), opened_(0) {}

    void 
    open();

    void 
    close();

    std::string const&
    filename() const { return filename_; }

    std::ifstream&
    file() { return file_; }

    std::ifstream const&
    file() const { return file_; }

    bool
    opened() const { return opened_; }

    private:
    std::string filename_;
    std::ifstream file_;
    bool opened_;
    };

std::ostream& 
operator<<(std::ostream &s, InputFile const& a);

/* 
To read in a table:

in input file:

tablename
    {
    a	x	y	z
    1	4.0	5.0	7.0
    2	4.0	2.0	7.0
    4	3.0	5.0	7.0
    5	4.0	5.0	1.0
    }

Then in program:
    InputGroup table(parent,"tablename");
    if(table.GotoGroup())
	{
	table.SkipLine(); //so the table can have a key
	for(int i = 1; i <= n; i++)
	    table.file() >> a[i] >> x[i] >> y[i] >> z[i];
	}
    */


class InputGroup
    {
    public:

    InputGroup() { }

    InputGroup(std::string filename, 
               std::string groupname,
               const char* c = 0);

    InputGroup(InputFile& inf, 
               std::string name,
               const char* c = 0);

    InputGroup(InputGroup& par, 
               std::string name,
               const char* c = 0);

    ~InputGroup();

    const std::string&
    name() const { return name_; }

    std::ifstream&
    file() const { return infile->file(); }

    // These versions return their value
    int getInt(std::string s, int default_);
    Real getReal(std::string s, Real default_);
    std::string getString(std::string s, std::string default_);
    bool getYesNo(std::string s, bool default_);

    int getInt(std::string s);
    Real getReal(std::string s);
    std::string getString(std::string s);
    bool getYesNo(std::string s);


    // The following go to s, and read into i,r,t, or yes
    int GetInt(std::string s, int& i, bool hasdf = false, int df = 0);
    int GetLong(std::string s, long& i, bool hasdf = false, long df = 0);
    int GetReal(std::string s, Real& r, bool hasdf = false, Real df = 0);	
    int GetString(std::string s, std::string& t, bool hasdf = false, std::string df = "");
    int GetYesNo(std::string s, int& yes, bool hasdf = false, int df = 0); // understands yes/no
    int GetYesNo(std::string s, bool& yes, bool hasdf = false, bool df = 0); // understands yes/no

    // The following are mandatory versions; if they doesn't get it, we quit
    void GetIntM(std::string s, int& i);	
    void GetLongM(std::string s, long& i);	
    void GetRealM(std::string s, Real& r);
    void GetStringM(std::string s, std::string& t);
    void GetYesNoM(std::string s, int& yes);

    void SkipLine();

    int GotoGroup();		// Goes to group, then eats "{" + whitespace
    int GotoToken(std::string s);	// Goes to Token, then eats "=" + whitespace

    private:

    optional_ptr<InputFile,std::shared_ptr<InputFile>> infile;
    InputGroup* parent;
    std::string name_;
    bool quiet;

    };

} //namespace itensor

#endif
