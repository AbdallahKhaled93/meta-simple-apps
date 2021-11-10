DESCRIPTION = "Toggle user leds"
PRIORITY = "optional" 
SECTION = "examples" 

LICENSE = "GPLv2" 
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/GPL-2.0;md5=801f80980d171dd6425610833a22dbe6" 

SRC_URI = "file://toggle-user-leds.cpp \
           file://daemon-user-led.cpp \
		   file://daemon-user-led"

S = "${WORKDIR}"
 
do_compile() {
	${CXX} ${CXXFLAGS} ${LDFLAGS} toggle-user-leds.cpp -o toggle-user-leds
	${CXX} ${CXXFLAGS} ${LDFLAGS} daemon-user-led.cpp -o daemon-user-led
} 

do_install() { 
	install -d ${D}${bindir}
	install -d ${D}${sysconfdir}/init.d
	install -d ${D}${sysconfdir}/rcS.d
	install -d ${D}${sysconfdir}/rc0.d
	install -d ${D}${sysconfdir}/rc6.d

	install -m 0755 toggle-user-leds ${D}${bindir}
	install -m 0755 daemon-user-led ${D}${bindir}
	install -m 0755 ${WORKDIR}/daemon-user-led ${D}${sysconfdir}/init.d

	ln -sf ../init.d/daemon-user-led ${D}${sysconfdir}/rcS.d/S98daemon-user-led
	ln -sf ../init.d/daemon-user-led ${D}${sysconfdir}/rc0.d/K00daemon-user-led
	ln -sf ../init.d/daemon-user-led ${D}${sysconfdir}/rc6.d/K00daemon-user-led
}
