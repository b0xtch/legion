#ifndef AUDIENCE_H_
#define AUDIENCE_H_

#include <string>
#include <utility>
// using std::unique_ptr

using pID = int;
using pName = std::string;

class Audience
{
public:
    Audience(pID, pName) : playerID(id), playerName(name){
                                             //include some check to ensure id is unique?
                                         };

    pID &getAudienceID()
    {
        return AudienceID;
    }
    pName &getAudienceName()
    {
        return AudienceName;
    }
    friend bool operator==(const Audience &a1, const Audience &a2)
    {
        return (
            a1.AudienceID == a2.AudienceID &&
            a1.AudienceName == a2.AudienceName);
    }

private:
    pID AudienceID;
    pName AudienceName;
    // networking::Connection connection; //Connections from PA, PB, PC to Game class for instantitations
};

#endif
