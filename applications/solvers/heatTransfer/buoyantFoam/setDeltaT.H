        // max Courant number hard coded
       scalar maxCo = 0.2;

        runTime.setDeltaT
        (
            min
            (
                maxCo*runTime.deltaT().value()/(CoNum + SMALL),
                1.1*runTime.deltaT().value()
            )
        );
