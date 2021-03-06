#include <eosio/asset.hpp>
#include <eosio/crypto.hpp>

#include <document_graph/document.hpp>
#include <document_graph/content_wrapper.hpp>

#include <common.hpp>
#include <util.hpp>
#include <proposals/edit_proposal.hpp>

namespace hypha
{

    void EditProposal::proposeImpl(const name &proposer, ContentWrapper &contentWrapper)
    { 
        // original_document is a required hash
    }

    void EditProposal::postProposeImpl (Document &proposal) 
    {
        ContentWrapper proposalContent = proposal.getContentWrapper();

        // confirm that the original document exists
        Document original (m_dao.get_self(), proposalContent.getOrFail(DETAILS, ORIGINAL_DOCUMENT)->getAs<eosio::checksum256>());

        // connect the edit proposal to the original
        Edge::write (m_dao.get_self(), m_dao.get_self(), proposal.getHash(), original.getHash(), common::ORIGINAL);
    }

    void EditProposal::passImpl(Document &proposal)
    {
        // merge the original with the edits and save
        ContentWrapper proposalContent = proposal.getContentWrapper();

        // confirm that the original document exists
        Document original (m_dao.get_self(), proposalContent.getOrFail(DETAILS, ORIGINAL_DOCUMENT)->getAs<eosio::checksum256>());

        // update all edges to point to the new document
        Document merged = Document::merge(original, proposal);
        merged.emplace ();

        // replace the original node with the new one in the edges table
        m_dao.getGraph().replaceNode(original.getHash(), merged.getHash());

        // erase the original document
        m_dao.getGraph().eraseDocument(original.getHash(), true);
    }

    std::string EditProposal::getBallotContent (ContentWrapper &contentWrapper)
    {
        return contentWrapper.getOrFail(DETAILS, TITLE)->getAs<std::string>();
    }
    
    name EditProposal::getProposalType () 
    {
        return common::EDIT;
    }

} // namespace hypha