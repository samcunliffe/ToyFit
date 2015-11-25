// Base class for all models
#ifndef MODEL_HPP
#define MODEL_HPP

#include <iostream>
#include <vector>

#include <TString.h>
#include <RooAbsPdf.h>
#include <RooRealVar.h>

class Model
{
 public:
  Model(TString name): _name(name) {}
  ~Model() {};
  virtual RooAbsPdf* pdf() = 0;
  void print();
  RooRealVar* getPar(TString);
 protected: 
  std::vector<RooRealVar*> _pars;
  TString _name;
 private:
};

//-----------------------------------------------------
// implementation of a couple of methods

void Model::print()
{ 
  std::cout << _name << " ";
  if (this->pdf())
    std::cout << this->pdf()->GetName();
  else
    std::cout << "RooAbsPdf pointer is null";
  std::cout << std::endl;
}
RooRealVar* Model::getPar(TString name)
{
  for (auto p: _pars)
    if (name == TString(p->GetName()))
      return p;
  return NULL;
}

#endif // MODEL_HPP
