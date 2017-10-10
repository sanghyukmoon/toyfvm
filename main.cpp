#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#define NGHOST 1

class Mesh {
public:
  Mesh();
  ~Mesh();
  // data
  int is,ie,nx1;
  double x1min, x1max, dx1;
  double *q, *f;
  int step, out_step;
  double start_time, final_time, cfl_number, lambda_max, time, dt;
  double out_time, out_dt;

  // function
  void Initialize();
  void OneStep();
  double Flux(double, double);
};

Mesh::Mesh()
{
  // TODO SMOON : I'm not familiar with string parser. Need improvement
  std::ifstream inputFile("inputs.txt");
  std::string line;
  if (inputFile.is_open())
  {
    std::getline(inputFile, line);
    x1min = std::stod(line);
    std::getline(inputFile, line);
    x1max = std::stod(line);
    std::getline(inputFile, line);
    nx1 = std::stoi(line);
    std::getline(inputFile, line);
    start_time = std::stod(line);
    std::getline(inputFile, line);
    final_time = std::stod(line);
    std::getline(inputFile, line);
    out_dt = std::stod(line);
    std::getline(inputFile, line);
    cfl_number = std::stod(line);
    std::getline(inputFile, line);
    lambda_max = std::stod(line);
  }
  else std::cout << "Unable to open 'inputs.txt'\n";

  is = NGHOST;
  ie = NGHOST+nx1-1;
  dx1 = (x1max - x1min) / (double)nx1;
  dt = dx1 * cfl_number / lambda_max;
  step = 0;
  out_step = 0;
  time = 0;
  out_time = 0;
}

void Mesh::Initialize()
{
  // SMOON : initial condition is set here. Better to take parameters from input file
  q = new double[nx1 + 2*NGHOST];
  f = new double[nx1 + 2*NGHOST + 1];
  for (int i=is; i<=ie; ++i) {
    double x1 = x1min + (i-is) * dx1;
    if ((x1 >= 0.2)&&(x1 <= 0.4)) q[i] = 1;
    else q[i] = 0;
  }
}

double Mesh::Flux(double ql, double qr)
{
  // SMOON : which numerical flux function to use
  //return lambda_max * ql;
  return 0.5 * lambda_max * (ql + qr);
}

void Mesh::OneStep()
{
  // fill ghost zones (periodic BC)
  for (int i=0; i<NGHOST; ++i) {
    q[i] = q[nx1 + i];
    q[nx1 + 2*NGHOST - 1 - i] = q[2*NGHOST - 1 - i];
  }
  for (int i=is; i<=ie+1; ++i) {
    f[i] = Flux(q[i-1], q[i]);
  }
  for (int i=is; i<=ie; ++i) {
    double dq = -1.0 * (cfl_number / lambda_max) * (f[i+1] - f[i]);
    q[i] += dq;
  }
  time += dt;
  step++;
}

void output(Mesh *pm)
{
  std::ofstream file;
  char filename[sizeof "out000.dat"];
  sprintf(filename, "out%03d.dat", pm->out_step);
  file.open(filename);
  for (int i=pm->is; i<=pm->ie; ++i) {
    double x1 = pm->x1min + (i-pm->is)*pm->dx1;
    file << x1 << " " << pm->q[i] << "\n";
  }
  file.close();
  pm->out_time += pm->out_dt;
  pm->out_step++;
}
  
int main()
{
  Mesh *pm;
  pm = new Mesh;
  pm->Initialize();
  while (pm->time < pm->final_time) {
    if (pm->time >= pm->out_time) output(pm);
    pm->OneStep();
  }
  return 0;
}
