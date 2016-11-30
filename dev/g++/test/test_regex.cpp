/*
#include <iostream>
#include <iomanip>
#include <cstring>

#include <regex>

using namespace std;

int main() {
  string nmea1("$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1,1.03,61.7,M,55.2,M,,*76\r\n");


  // Extract command
  string _commandPattern("^\\$GP(\\w{3}),(.*),([\\w\\d]*)\\*([:xdigit:]+)\r\n^");
  regex _command(_commandPattern);
  
  smatch matches;
  regex_match(nmea1, matches, _command);
  cout << "match results: " << matches.size();
  
  return 0;
  }*/
#include <iostream>
#include <iomanip>
#include <cstring>
//#include <sys/types.h>
#include <regex.h>

using namespace std;

int main(int argc, char *argv[]){
  regex_t regexCommand;
  regex_t regex;
  int reti;
  char msgbuf[100];
  regmatch_t *pmatches;

  string nmea1("$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1,1.03,61.7,M,55.2,M,,A*76\r\n");
  string nmea2("$GPRMC,134609.00,A,4339.80690,N,00642.70870,E,0.215,,130913,,,A*74\r\n");

  //  string nmea1("$GPGGA,,5321.6802,N,00630.3372,W,1,8,1,1.03,61.7,M,55.2,M,,*76\r\n");
  // Extract command
  string _commandPattern(
			 "^\\$GP([[:alpha:]]{3}),"
			 "(.*)"
			 "\r\n$");
  string _ggaPattern(
		     "([[:digit:]]+\\.[[:digit:]]+){0,1},"
		     "([[:digit:]]+\\.[[:digit:]]+){0,1},"
		     "([NS]){0,1},"
		     "([[:digit:]]+\\.[[:digit:]]+){0,1},"
		     "([WE]){0,1},"
		     "([[:digit:]]*),"
		     "([[:digit:]]*),"
		     "([[:digit:]]*),"
		     "([[:digit:]]+\\.[[:digit:]]+){0,1},"
		     "([[:digit:]]+\\.[[:digit:]]+){0,1},"
		     "([M]){0,1},"
		     "([[:digit:]]+\\.[[:digit:]]+){0,1},"
		     "([M]){0,1},"
		     "(.*),"
		     "([[:xdigit:]]*)\\*([[:xdigit:]]{2})"
		     );

  // Compile regular expression
  if (regcomp(&regexCommand, _commandPattern.c_str(), REG_EXTENDED | REG_NEWLINE) != 0) {
    cerr << "Could not compile regex" << endl; 
    exit(1); 
  }
  if (regcomp(&regex, _ggaPattern.c_str(), REG_EXTENDED | REG_NEWLINE) != 0) { 
    cerr << "Could not compile regex" << endl; 
    exit(1); 
  }

  // Extract command
  pmatches = (regmatch_t *)new unsigned char[sizeof(regmatch_t) * (1 + regexCommand.re_nsub)];
  if (regexec(&regexCommand, nmea1.c_str(), 1 + regexCommand.re_nsub, pmatches, 0) != 0) {
    delete [] pmatches;
    exit(2);
  }
  size_t matchLen = (pmatches + 2)->rm_eo - (pmatches + 2)->rm_so;
  cout << "pmatches->rm_eo=" << (pmatches + 2)->rm_eo << " - pmatches->rm_so="  << (pmatches + 2)->rm_so << " = " << matchLen << endl;
  string payload = nmea1.substr((pmatches + 2)->rm_so, matchLen);
  cout << "payload ='" << payload << "'" << endl;
  delete [] pmatches;

  /* Execute regular expression */
  pmatches = (regmatch_t *)new unsigned char[sizeof(regmatch_t) * (1 + regex.re_nsub)];
  reti = regexec(&regex, payload.c_str(), 1 + regex.re_nsub, pmatches, 0);
  if( !reti ){
    cout << "Match" << endl;
    if (pmatches->rm_eo != -1) { // Matching
      char *pstr = (char *)payload.c_str();
      size_t counter = 0;
      do {
	  char *submatch;
	  size_t matchLen = (pmatches + counter)->rm_eo - (pmatches + counter)->rm_so;
	  //	  cout << "pmatches->rm_eo=" << (pmatches + counter)->rm_eo << " - pmatches->rm_so="  << (pmatches + counter)->rm_eo << " = " << matchLen << endl;
	  submatch = (char *)new char[matchLen + 1];
	  strncpy(submatch, pstr + (pmatches + counter)->rm_so, matchLen + 1);
	  submatch[matchLen] = '\0';
	  cout << "Item #" << counter << " = '" << submatch << "'" << endl;
	  delete [] submatch;
	  counter += 1;
      } while(counter < regex.re_nsub + 1);
    }
  }
  else if( reti == REG_NOMATCH ){
    cout << "No match" << endl;
  }
  else{
    delete [] pmatches;
    regerror(reti, &regex, msgbuf, sizeof(msgbuf));
    cerr << "Regex match failed: " << msgbuf << endl;
    exit(1);
  }

  /* Free compiled regular expression if you want to use the regex_t again */
  delete [] pmatches;
  regfree(&regex);

  return 0;
}
