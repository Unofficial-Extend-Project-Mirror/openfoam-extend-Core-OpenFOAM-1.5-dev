pointField mPoints = mSolver.curPoints();

forAll(mPoints, pointI)
{
    if(mPoints[pointI].z() < pistonPosition_ )
    {
        pistonBowlPoints.append(pointI);
    }
}

vectorField pointsVelocity(labelList(pistonBowlPoints.shrink()).size());

forAll(pointsVelocity, pointI)
{
    pointsVelocity[pointI] = vector::zero;
}
