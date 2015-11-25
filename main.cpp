
#include <vector>
#include <iostream>

#include <TString.h>
#include <TCanvas.h>

#include <RooFitResult.h>
#include <RooFit.h>
#include <RooAbsPdf.h>
#include <RooGaussian.h>
#include <RooPlot.h>
#include <RooMinuit.h>
#include <RooRealVar.h>
#include <RooArgSet.h>
#include <RooDataSet.h>

const bool DEBUG = 1;

class Model
{
 public:
  Model(TString name): _name(name) {}
  ~Model() {};
  virtual RooAbsPdf* pdf() = 0;
  void print()
  { 
    std::cout << _name << " ";
    if (this->pdf())
      std::cout << this->pdf()->GetName();
    else
      std::cout << "RooAbsPdf pointer is null";
    std::cout << std::endl;
  }
  RooRealVar* getPar(TString name)
  {
    for (auto p: _pars)
      if (name == TString(p->GetName()))
        return p;
    return NULL;
  }
 protected: 
  std::vector<RooRealVar*> _pars;
 private:
  TString _name;
};

class GaussianModel1D : public Model
{
 public:
  GaussianModel1D(TString name, RooRealVar &x):
      Model(name),
      _x(x)
  {
    RooRealVar* m = new RooRealVar("mean", "#bar{x}", 0, -1, 1);
    RooRealVar* s = new RooRealVar("sigma", "#sigma", 0.2, 0.1, 1.0);
    _pars.push_back(m);
    _pars.push_back(s);
    _pdf = new RooGaussian(name+"_pdf", name, _x, *m, *s);
  }
  ~GaussianModel1D()
  {
    delete _pdf;
    for (auto p: _pars)
      delete p;
  }
  RooDataSet* generate(int n=10000)
  {
    return _pdf->generate(RooArgSet(_x), n);
  }
  RooAbsPdf* pdf() { return _pdf; }

 private:
  RooRealVar& _x;
  RooGaussian* _pdf;
};

class Fitter
{
 public:
  Fitter(Model* m, RooDataSet* d): 
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
void Fitter::fit()
{
  if (DEBUG) {
    std::cout << "Fitter::fit _nll = ";
    std::cout << _nll << std::endl;
  }
  namespace rf=RooFit; // shorthand for the namespace
  RooMinuit minimiser(*_nll);
  _migrad = minimiser.migrad(); // call migrad, catch its return code
  _hesse  = minimiser.hesse();  // call hesse, catch its return code
  _rfr    = minimiser.save();
  _nll->Print();
  std::cout << _nll << std::endl;
  return;
}
void Fitter::plotNLLScan(RooRealVar* par)
{
  //std::cout << _pdf << std::endl;
  std::cout << _nll << std::endl;
  std::cout << _rfr << std::endl;
  // just a simple scan of the NLL
  if (!_nll) {
    std::cout << "Fitter::plotNLLScan ERROR you should fit first,"
        " otherwise the NLL is not minimised" << std::endl;
    return;
  }
  RooPlot* rp = par->frame();
  _nll->plotOn(rp);
  TCanvas canvas;
  rp->Draw();
  canvas.SaveAs(TString(par->GetName())+"_nll_scan.pdf");
  return;      
}

class Plotter1D
{
 public:
  Plotter1D(RooRealVar &x): _x(x) {}
  void plot(GaussianModel1D& model, RooDataSet* data)
  {
    RooPlot* rp = _x.frame();
    data->plotOn(rp);
    model.pdf()->plotOn(rp);
    TCanvas canvas;
    rp->Draw();
    canvas.SaveAs("plot.pdf");
    return;
  }
 private:
  RooRealVar& _x;
};

int main(int argc, char *argv[])
{
  // setup model
  RooRealVar x("x", "x", -1, 1);
  GaussianModel1D model("simple", x);
  RooDataSet *data = model.generate();

  // minimise the likelihood
  Fitter f(&model, data);
  std::cout << "pointer to the model pdf " << model.pdf() << std::endl;
  f.fit();
  f.plotNLLScan(model.getPar("mean")); // make a quick NLL plot
  f.plotNLLScan(model.getPar("sigma"));

  // make a plot of the PDF and data
  Plotter1D p(x);
  p.plot(model, data);

  return 0;
}
