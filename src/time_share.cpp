#include "time_share.hpp"

#include <dao.hpp>
#include <common.hpp>

namespace hypha {

TimeShare::TimeShare(name contract, name creator, int64_t timeShare, time_point startDate) 
: Document(contract, creator, constructContentGroups(timeShare, startDate))
{
  
}

TimeShare::TimeShare(name contract, eosio::checksum256 hash)
: Document(contract, hash)
{
  
}

std::optional<TimeShare> TimeShare::getNext(name contract)
{
  if (auto [exists, edge] = Edge::getIfExists(contract, getHash(), common::NEXT_TIME_SHARE);
      exists) {
    return TimeShare(contract, edge.getToNode());
  }

  return std::nullopt;
}

ContentGroups TimeShare::constructContentGroups(int64_t timeShare, time_point startDate) 
{
  return {
    ContentGroup {
      Content(CONTENT_GROUP_LABEL, DETAILS),
      Content(TIME_SHARE, timeShare),
      Content(TIME_SHARE_START_DATE, startDate),
    },
    ContentGroup {
      Content(CONTENT_GROUP_LABEL, SYSTEM),
      Content(TYPE, common::TIME_SHARE_LABEL),
      Content(NODE_LABEL, common::TIME_SHARE_LABEL.to_string()),
    }
  };
}

}