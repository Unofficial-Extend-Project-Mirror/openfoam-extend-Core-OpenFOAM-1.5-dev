{
    if (!memberNumber)
    {
        timeOut
            << "object \"series\" class series"
            << endl;
    }

    timeOut
        << "member " << memberNumber
        << " position " << Times[i].value() << " value"
        << " file \"" << Times[i].name()
        << ".data\"," << "\"FIELDS\""
        << endl;

    memberNumber++;

    if (memberNumber == Times.size())
    {
        timeOut
            << "\nend" << endl;
    }
}
