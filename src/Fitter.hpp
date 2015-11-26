// Base class for a fitter. Only tested in 1D so far.
#ifndef FITTER_HPP
#define FITTER_HPP

#include <RooAbsReal.h> // the NLL object is a RAR
#include <RooDataSet.h>
#include <RooFitResult.h>
#include "Model.hpp"

class Fitter
{
 public:
  Fitter(Model* m, /*const*/ RooDataSet* d): 
      _model(m), _data(d), _rfr(NULL), 
      _nll(_model->pdf()->createNLL(*_data)),
      _migrad(-1), _hesse(-1), _minos(-1) { }
  ~Fitter()
  {
    delete _rfr;
    delete _nll;
  }
  void fit();
  void plotNLLScan(RooRealVar* x);
  void plotNLLScan(RooRealVar* x, RooRealVar* y);
  void plotNLLProfile(RooRealVar* x);
  void plotNLLProfile(RooRealVar* x, RooRealVar* y);
 private:
  Model* _model;
  RooFitResult* _rfr;
  RooDataSet* _data;
  RooAbsReal* _nll;
  int _migrad, _hesse, _minos;
};

#endif // FITTER_HPP
