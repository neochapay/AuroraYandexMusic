Name:       ru.neochapay.ourmusic
Summary:    The unofficial client for Yandex Music
Version:    0.3.5
Release:    0
Group:      Qt/Qt
License:    GNU GPLv3
URL:        https://github.com/neochapay/AuroraYandexMusic
Source0:    %{name}-%{version}.tar.bz2
Requires:   sailfishsilica-qt5 >= 0.10.9
BuildRequires:  pkgconfig(sailfishapp) >= 1.0.2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  desktop-file-utils
BuildRequires:  librsvg-tools

%description
Yandex Music (unofficial) for Aurora OS


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


for size in 86 108 128 172 256
do
   mkdir -p %{buildroot}%{_datadir}/icons/hicolor/${size}x${size}/apps/
   rsvg-convert --width=$size --height=$size --output \
           %{buildroot}%{_datadir}/icons/hicolor/${size}x${size}/apps/%{name}.png \
           %{_sourcedir}/../icons/svg/ru.neochapay.ourmusic.svg
done

if grep "Sailfish" /etc/os-release; then
    rm %{buildroot}%{_datadir}/applications/ru.neochapay.ourmusic.desktop
    cp  %{_sourcedir}/../ru.neochapay.ourmusic.desktop_nojail %{buildroot}%{_datadir}/applications/ru.neochapay.ourmusic.desktop
fi

%files
%defattr(-,root,root,-)
%{_bindir}/%{name}
%defattr(644,root,root,-)
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/apps/%{name}.png
