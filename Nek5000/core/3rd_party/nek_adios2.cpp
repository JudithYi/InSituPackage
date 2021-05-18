#include <adios2.h>
#include <string>


adios2::ADIOS adios;
adios2::IO io;
adios2::Engine writer;
adios2::Variable<double> x;
adios2::Variable<double> y;
adios2::Variable<double> z;
adios2::Variable<double> p;
adios2::Variable<double> vx;
adios2::Variable<double> vy;
adios2::Variable<double> vz;
adios2::Variable<double> t;

extern "C" void adios2_setup_(
    const int *dim
){
    std::string configFile="config/config.xml";
    adios = adios2::ADIOS(configFile, MPI_COMM_WORLD);
    io = adois.DeclareIO("writer");
    if (!io.InConfigFile())
    {
        // if not defined by user, we can change the default settings
        // BPFile is the default engine
        io.SetEngine("BPFile");
        io.SetParameters({{"num_threads", "1"}});

        // ISO-POSIX file output is the default transport (called "File")
        // Passing parameters to the transport
#ifdef _WIN32
        io.AddTransport("File", {{"Library", "stdio"}});
#else
        io.AddTransport("File", {{"Library", "posix"}});
#endif
    }
    x = io.DefineVariable<double>("X", {}, {}, {adios2::UnknownDim});
    y = io.DefineVariable<double>("Y", {}, {}, {adios2::UnknownDim});
    if(*dim == 3) z = io.DefineVariable<double>("Z", {}, {}, {adios2::UnknownDim});
    p = io.DefineVariable<double>("P", {}, {}, {adios2::UnknownDim});
    vx = io.DefineVariable<double>("VX", {}, {}, {adios2::UnknownDim});
    vy = io.DefineVariable<double>("VY", {}, {}, {adios2::UnknownDim});
    if(*dim == 3) vz = io.DefineVariable<double>("VZ", {}, {}, {adios2::UnknownDim});
    t = io.DefineVariable<double>("T", {}, {}, {adios2::UnknownDim});
    writer = io.Open("localArray.bp", adios2::Mode::Write);
}

extern "C" void adios2_update_(
    const double *xml,
    const double *yml,
    const double *pr,
    const double *v,
    const double *u,
    const double *temp,
    const int *nval
){
    writer.BeginStep();
    x.SetSelection(adios2::Box<adios2::Dims>({}, {*nval}));
    writer.Put<double>(x, xml);
    y.SetSelection(adios2::Box<adios2::Dims>({}, {*nval}));
    writer.Put<double>(y, yml);
    p.SetSelection(adios2::Box<adios2::Dims>({}, {*nval}));
    writer.Put<double>(p, pr);
    vx.SetSelection(adios2::Box<adios2::Dims>({}, {*nval}));
    writer.Put<double>(vx, v);
    vy.SetSelection(adios2::Box<adios2::Dims>({}, {*nval}));
    writer.Put<double>(vy, u);
    t.SetSelection(adios2::Box<adios2::Dims>({}, {*nval}));
    writer.Put<double>(t, temp);
    writer.EndStep();
}

extern "C" void adios2_update_(
    const double *xml,
    const double *yml,
    const double *zml,
    const double *pr,
    const double *v,
    const double *u,
    const double *w,
    const double *temp,
    const int *nval
){
    writer.BeginStep();
    x.SetSelection(adios2::Box<adios2::Dims>({}, {*nval}));
    writer.Put<double>(x, xml);
    y.SetSelection(adios2::Box<adios2::Dims>({}, {*nval}));
    writer.Put<double>(y, yml);
    z.SetSelection(adios2::Box<adios2::Dims>({}, {*nval}));
    writer.Put<double>(z, yml);
    p.SetSelection(adios2::Box<adios2::Dims>({}, {*nval}));
    writer.Put<double>(p, pr);
    vx.SetSelection(adios2::Box<adios2::Dims>({}, {*nval}));
    writer.Put<double>(vx, v);
    vy.SetSelection(adios2::Box<adios2::Dims>({}, {*nval}));
    writer.Put<double>(vy, u);
    vz.SetSelection(adios2::Box<adios2::Dims>({}, {*nval}));
    writer.Put<double>(vz, w);
    t.SetSelection(adios2::Box<adios2::Dims>({}, {*nval}));
    writer.Put<double>(t, temp);
    writer.EndStep();
}

extern "C" void adios2_finalize_(){
    writer.Close();
}