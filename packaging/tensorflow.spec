Name:       tensorflow
Summary:    Tensorflow
Version:    v1.5.0
Release:    1
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz

BuildRequires: pkgconfig(zlib)
BuildRequires: pkgconfig(gmock)

%description
TensorFlow is an open source software library for numerical computation using data flow graphs.

%package -n tensorflow-lite-devel
Version: dev_preview
Summary: Tensorflow Lite development headers and object file

%description -n tensorflow-lite-devel
Tensorflow Lite development headers and object file

%prep
%setup -q

%build
# build for tensorflow mobile
#JOB_COUNT=4 TIZEN_FLAGS="${CXXFLAGS}" tensorflow/contrib/makefile/build_all_linux.sh

# build for tensorflow lite
%ifarch arm armv7l aarch64
make -f tensorflow/contrib/lite/Makefile TARGET=TIZEN OS=LINUX TIZEN_CXXFLAGS="${CXXFLAGS} -D__ARM_NEON" TIZEN_CFLAGS="${CFLAGS} -D__ARM_NEON"
%else
make -f tensorflow/contrib/lite/Makefile TARGET=TIZEN OS=LINUX TIZEN_CXXFLAGS="${CXXFLAGS}" TIZEN_CFLAGS="${CFLAGS}"
%endif

%install

#install for tensorflow lite
mkdir -p %{buildroot}%{_libdir}
mkdir -p %{buildroot}%{_libdir}/pkgconfig
mkdir -p %{buildroot}%{_includedir}/tensorflow/contrib/lite
mkdir -p %{buildroot}%{_includedir}/tensorflow/contrib/lite/kernels
mkdir -p %{buildroot}%{_includedir}/tensorflow/contrib/lite/tools
mkdir -p %{buildroot}%{_includedir}/tensorflow/contrib/lite/schema
mkdir -p %{buildroot}%{_includedir}/flatbuffers

sed -i 's:${libdir}:%{_libdir}:g
	s:${includedir}:%{_includedir}:g' tensorflow/contrib/lite/tensorflow-lite.pc.in

install -m 0644 tensorflow/contrib/lite/gen/lib/libtensorflow-lite.a %{buildroot}%{_libdir}/
install -m 0644 tensorflow/contrib/lite/*.h %{buildroot}%{_includedir}/tensorflow/contrib/lite/
install -m 0644 tensorflow/contrib/lite/kernels/*.h %{buildroot}%{_includedir}/tensorflow/contrib/lite/kernels
install -m 0644 tensorflow/contrib/lite/tools/*.h %{buildroot}%{_includedir}/tensorflow/contrib/lite/tools
install -m 0644 tensorflow/contrib/lite/schema/*.h %{buildroot}%{_includedir}/tensorflow/contrib/lite/schema
install -m 0644 tensorflow/contrib/lite/downloads/flatbuffers/include/flatbuffers/*.h %{buildroot}%{_includedir}/flatbuffers
install -m 0644 tensorflow/contrib/lite/tensorflow-lite.pc.in %{buildroot}%{_libdir}/pkgconfig/tensorflow-lite.pc

%files
%license LICENSE

%files -n tensorflow-lite-devel
%{_libdir}/libtensorflow-lite.a
%{_libdir}/pkgconfig/tensorflow-lite.pc
%{_includedir}/tensorflow/contrib/lite
%{_includedir}/flatbuffers

