Name:       tensorflowmobile
Summary:    Tensorflow Mobile
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
BuildRequires: pkgconfig(gmock)

%description
Tensorflow Mobile

%prep
%setup -q

%build
#tensorflow/contrib/lite/download_dependencies.sh
#make -f tensorflow/contrib/lite/Makefile TARGET=TIZEN TIZEN_CXXFLAGS="${CXXFLAGS} -D__ARM_NEON" TIZEN_CFLAGS="${CFLAGS} -D__ARM_NEON"
JOB_COUNT=4 TIZEN_FLAGS="${CXXFLAGS}" tensorflow/contrib/makefile/build_all_linux.sh

%install

%files


