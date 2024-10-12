/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    gen6DoF

Description
    Generate simple sinusoidal 6-DoF motion control-file.

\*---------------------------------------------------------------------------*/

#include "List.H"
#include "vector.H"
#include "Vector2D.H"
#include "Tuple2.H"
#include "OFstream.H"
#include "fvCFD.H"

using namespace Foam;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Main program:
int main(int argc, char *argv[])
{
    #include "setRootCaseLists.H"
    #include "createTime.H"
    #include "createMesh.H"

    // Read end time of the table from control dict
    const scalar endTime = runTime.controlDict().get<scalar>("endTime");


    // Read deltaT from control dict
    const scalar deltaT = runTime.controlDict().get<scalar>("deltaT");

    // Number of entries in the table
    const label nTimes = endTime / deltaT;

    Info<< "Generating " << nTimes << " values to 6DOF table until end time of " << endTime << endl;

    IOdictionary flappingProperties
    (
            IOobject
            (
                "flappingProperties",
                runTime.constant(),
                mesh,
                IOobject::MUST_READ,
                IOobject::NO_WRITE
            )
    );

    // Amplitude of the rotation [deg]
    const scalar rotAmp = flappingProperties.get<scalar>("amplitude");

    // Frequency of the rotation [Hz]
    const scalar rotFrequ = flappingProperties.get<scalar>("frequency");

    List<Tuple2<scalar,  Vector2D<vector>>> timeValues(nTimes);

    forAll(timeValues, i)
    {
        scalar t = (endTime*i)/(nTimes - 1);
        timeValues[i].first() = t;

        timeValues[i].second()[0] = vector(0, 0, 0);

        timeValues[i].second()[1] = vector
        (
            0,
            0,
            rotAmp*Foam::sin(2*constant::mathematical::pi*rotFrequ*t)
        );
    }

    {
        OFstream dataFile("constant/6DoF.dat");
        dataFile << timeValues << endl;
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
