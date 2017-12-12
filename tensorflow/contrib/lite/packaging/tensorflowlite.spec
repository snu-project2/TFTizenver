Name:       tensorflowlite
Summary:    Tensorflow Lite
Version:    0.1
Release:    1
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz

BuildRequires: curl
BuildRequires: tar
BuildRequires: gzip
BuildRequires: unzip
BuildRequires: openssl
BuildRequires: ca-certificates
BuildRequires: libgnutls
BuildRequires: pkgconfig(zlib)

%description
Tensorflow Lite

%prep
%setup -q

%build
#tensorflow/contrib/lite/download_dependencies.sh
make -f tensorflow/contrib/lite/Makefile TARGET=TIZEN TIZEN_CXXFLAGS="${CXXFLAGS}" TIZEN_CFLAGS="${CFLAGS}"

%install

%files


