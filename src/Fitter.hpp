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
      _model(m), _data(d), _rfr(NULL), // _nll(NULL), 
        _nll(_model->pdf()->createNLL(*_data)),
      _migrad(0), _hesse(0), _minos(0) { }
  ~Fitter()
  {
    delete _rfr;
    delete _nll;
  }
  void fit();
  void plotNLLScan(RooRealVar*);
  void plotNLLProfile()
  {
    // Implement me
    return;
  }
 private:
  Model* _model;
  RooFitResult* _rfr;
  RooDataSet* _data;
  RooAbsReal* _nll;
  int _migrad, _hesse, _minos;
};

#endif // FITTER_HPP
