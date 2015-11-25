// A simple 1D Gaussian pdf with domain over x in [-1, 1] by default
#ifndef GAUSSIANMODEL1D_HPP
#define GAUSSIANMODEL1D_HPP

#include <RooDataSet.h>
#include <RooGaussian.h>

#include "Model.hpp"

class GaussianModel1D : public Model
{
 public:
  GaussianModel1D(TString name, RooRealVar &x):
      Model(name),
      _x(x)
  {
    this->build();
  }
  ~GaussianModel1D()
  {
    delete _pdf;
    for (auto p: _pars)
      delete p;
  }
  RooDataSet* generate(int n=10000)
  { return _pdf->generate(RooArgSet(_x), n); }
  RooAbsPdf* pdf() { return _pdf; }

 private:
  void build();
  RooRealVar& _x;
  RooGaussian* _pdf;
};

void GaussianModel1D::build()
{
  RooRealVar* m = new RooRealVar("mean", "#bar{x}", 0, -1, 1);
  RooRealVar* s = new RooRealVar("sigma", "#sigma", 0.2, 0.1, 1.0);
  _pars.push_back(m);
  _pars.push_back(s);
  _pdf = new RooGaussian(_name+"_pdf", _name, _x, *m, *s);
  return;
}

#endif // GAUSSIANMODEL1D_HPP
