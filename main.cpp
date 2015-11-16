
#include <vector>
#include <iostream>

#include <TString.h>
#include <TCanvas.h>

#include <RooFitResult.h>
#include <RooFit.h>
#include <RooAbsPdf.h>
#include <RooGaussian.h>
#include <RooPlot.h>
#include <RooRealVar.h>
#include <RooArgSet.h>
#include <RooDataSet.h>

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
  private:
   TString _name;
};

class GaussianModel1D : Model
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
  std::vector<RooRealVar*> _pars;
};

class Fitter
{
 public:
  void fit(GaussianModel1D& model, RooDataSet* data)
  {
    namespace rf=RooFit;
    _rfr = model.pdf()->fitTo(*data, rf::Save(true));
    return;
  }
 private:
  RooFitResult* _rfr;
};

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
  RooRealVar x("x", "x", -1, 1);
  GaussianModel1D model("simple", x);
  RooDataSet *data = model.generate();
  Fitter f;
  f.fit(model, data);
  Plotter1D p(x);
  p.plot(model, data);
  return 0;
}
