#include "Utilities/General/interface/CMSexception.h"
#include "Utilities/General/interface/UncatchedException.h"
#include "Utilities/General/interface/ioutils.h"

LockMutex::Mutex UncatchedException::mutex;
int UncatchedException::count_=0; 
cms::Exception * UncatchedException::it=0;

UncatchedException::UncatchedException() {
  LockMutex a(mutex);
  count_++;
}

UncatchedException::UncatchedException(const cms::Exception & err) {
  LockMutex a(mutex);
  count_++;
  //this is bad but needed in order to make transition
  it = new cms::Exception(err);
}

void UncatchedException::dump(std::ostream & o, bool det) {
  if (!it) return;
  Genexception * exp = dynamic_cast<Genexception*>(it);
  if (exp) exp->dump(o,det);
  else o << it->what() << std::endl;
}

void UncatchedException::rethrow() {
  if (count()==0) return;
  std::string mess("found "); mess+=toa()(count());
  mess+= " uncaught exceptions";
  if (!it) throw Genexception(mess);

  //bad but needed in order to make transition to standard CMS exceptions
  throw *it;
  //it->rethrow();
  
}

int UncatchedException::count() {
  LockMutex a(mutex);
  return count_;
}

//-------------------------------------------------------------------

BaseGenexception::BaseGenexception() throw() {}


BaseGenexception::BaseGenexception(const std::string & mess) throw() : 
  message(mess) {}

BaseGenexception::~BaseGenexception() throw() {
  /* cout << "deleting a BaseGenexception " << message << " at " << this << endl; */
}

//-------------------------------------------------------------------

Genexception::Genexception() throw() {traceit();}


Genexception::Genexception(const std::string & mess) throw() : 
  BaseGenexception(mess) {
  traceit();
}


Genexception::~Genexception() throw() {}

void Genexception::add(Genexception * in) throw() {
  if (in==0) return;
  // push in the stack...
  if (next.get()) (*next).add(in);
  else next = own_ptr<Genexception>(in);
}

#include<iostream>
void Genexception::dump(std::ostream & o, bool it)  const throw() {
  if (next.get()) {
    (*next).dump(o,it);
    o << "Generated by: " ;
  }
  o << what() << "\n" << std::endl;
  if(it) o << trace()<< std::endl;
}

#include "Utilities/General/interface/BackTrace.h"
#include<sstream>

void Genexception::traceit() throw() {
  std::ostringstream oss;

  BackTrace a;
  a.trace(oss);
  trace_ = oss.str();
}

namespace Capri {
  namespace details {
    static std::string from(" from ");
  }
}

Capri::Error::Error(const std::string & level, const std::string & mess) throw() :
  Genexception(level+details::from+mess) {}


GenTerminate::GenTerminate() throw() {}
GenTerminate::GenTerminate(const std::string & mess) throw() : Genexception(mess) {}
GenTerminate::~GenTerminate() throw() {}


Success::Success(){}
Success::~Success(){}