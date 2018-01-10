Name:       tensorflowlite
Summary:    Tensorflow Lite
Version:    0.1
Release:    1
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz

#BuildRequires: curl
#BuildRequires: tar
#BuildRequires: gzip
#BuildRequires: unzip
#BuildRequires: openssl
#BuildRequires: ca-certificates
#BuildRequires: libgnutls
BuildRequires: pkgconfig(zlib)

%description
Tensorflow Lite

%package devel
Summary: Tensorflow Lite

%description devel
Tensorflow Lite static library and headers

%prep
%setup -q

%build
#tensorflow/contrib/lite/download_dependencies.sh
make -f tensorflow/contrib/lite/Makefile TARGET=TIZEN TIZEN_CXXFLAGS="${CXXFLAGS} -D__ARM_NEON" TIZEN_CFLAGS="${CFLAGS} -D__ARM_NEON"

%install
mkdir -p %{buildroot}%{_libdir}
mkdir -p %{buildroot}%{_libdir}/pkgconfig
mkdir -p %{buildroot}%{_includedir}/tensorflow/contrib/lite
mkdir -p %{buildroot}%{_includedir}/tensorflow/contrib/lite/kernels
mkdir -p %{buildroot}%{_includedir}/tensorflow/contrib/lite/tools

sed -i 's:${libdir}:%{_libdir}:g
	s:${includedir}:%{_includedir}:g' tensorflow/contrib/lite/packaging/tensorflowlite.pc.in

install -m 0644 tensorflow/contrib/lite/gen/lib/libtensorflow-lite.a %{buildroot}%{_libdir}/
install -m 0644 tensorflow/contrib/lite/*.h %{buildroot}%{_includedir}/tensorflow/contrib/lite/
install -m 0644 tensorflow/contrib/lite/kernels/*.h %{buildroot}%{_includedir}/tensorflow/contrib/lite/kernels
install -m 0644 tensorflow/contrib/lite/tools/*.h %{buildroot}%{_includedir}/tensorflow/contrib/lite/tools
install -m 0644 tensorflow/contrib/lite/packaging/tensorflowlite.pc.in %{buildroot}%{_libdir}/pkgconfig/tensorflowlite.pc


%files

%files devel
%{_libdir}/libtensorflow-lite.a
%{_libdir}/pkgconfig/tensorflowlite.pc
%{_includedir}/tensorflow/contrib/lite

