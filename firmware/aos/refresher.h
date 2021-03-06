/*$file${.::refresher.h} ###################################################*/
/*
* Model: refrsher.qm
* File:  ${.::refresher.h}
*
* This code has been generated by QM tool (https://state-machine.com/qm).
* DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*/
/*$endhead${.::refresher.h} ################################################*/
#ifndef REFRESHER_H
#define REFRESHER_H

#include"qpn.h"

/* define the event signals used in the application ------------------------*/
enum RefresherSignals {
    R_REFRESH_CMD_SIG = Q_USER_SIG,
    R_STANDBY_SIG,
    R_SET_INTERVAL_CMD_SIG,
    R_MAX_SIG /* the last signal */
};

/* active object(s) used in this application -------------------------------*/
extern struct Refresher AO_Refresher; /* opaque struct */
/*$declare${AOs::Refresher_ctor} ###########################################*/
/*${AOs::Refresher_ctor} ...................................................*/
void Refresher_ctor(void);
/*$enddecl${AOs::Refresher_ctor} ###########################################*/

#endif /* REFRESHER_H */