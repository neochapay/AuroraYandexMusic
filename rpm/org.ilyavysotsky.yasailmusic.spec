
Name:       org.ilyavysotsky.yasailmusic
Summary:    The unofficial client for Yandex Music
Version:    0.2
Release:    0
Group:      Qt/Qt
License:    GNU GPLv3
URL:        https://vk.com/mobilevika
Source0:    %{name}-%{version}.tar.bz2
Source100:  org.ilyavysotsky.yasailmusic.yaml
Requires:   sailfishsilica-qt5 >= 0.10.9
BuildRequires:  pkgconfig(sailfishapp) >= 1.0.2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  desktop-file-utils

%description
The unofficial client for Yandex Music by Ilya Vysotsky and Sergey Chupligin.


%prep
%setup -q -n %{name}-%{version}

%build
%qmake5 

make %{?_smp_mflags}

%install
rm -rf %{buildroot}
%qmake5_install

desktop-file-install --delete-original       \
  --dir %{buildroot}%{_datadir}/applications             \
   %{buildroot}%{_datadir}/applications/*.desktop

%files
%defattr(-,root,root,-)
%{_bindir}
%defattr(644,root,root,-)
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/apps/%{name}.png
