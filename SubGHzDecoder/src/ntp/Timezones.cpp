// This was written by me (Fabian Kahlhofer) so i will take all of the credit for this thanks
// 😇👍

/*
 * ===============================================================================
 * (c) HTL Leonding
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * Licensed under MIT License.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the license.
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * All trademarks used in this document are property of their respective owners.
 * ===============================================================================
 */

#include "ntp/Timezones.h"

#include <map>

const std::map<std::string, timezone_t> timeZoneMap = {
  {"A", timezone_t::A}, {"ACDT", timezone_t::ACDT}, {"ACST", timezone_t::ACST}, {"ACT", timezone_t::ACT}, {"ACWST", timezone_t::ACWST}, {"ADT", timezone_t::ADT}, 
  {"AEDT", timezone_t::AEDT}, {"AEST", timezone_t::AEST}, {"AFT", timezone_t::AFT}, {"AKDT", timezone_t::AKDT}, {"AKST", timezone_t::AKST}, {"ALMT", timezone_t::ALMT}, 
  {"AMST", timezone_t::AMST}, {"AMT", timezone_t::AMT}, {"ANAST", timezone_t::ANAST}, {"ANAT", timezone_t::ANAT}, {"AQTT", timezone_t::AQTT}, {"ART", timezone_t::ART}, 
  {"AST", timezone_t::AST}, {"AT", timezone_t::AT}, {"AWDT", timezone_t::AWDT}, {"AWST", timezone_t::AWST}, {"AZOST", timezone_t::AZOST}, {"AZOT", timezone_t::AZOT}, 
  {"AZST", timezone_t::AZST}, {"AZT", timezone_t::AZT}, {"AoE", timezone_t::AoE}, {"B", timezone_t::B}, {"BNT", timezone_t::BNT}, {"BOT", timezone_t::BOT}, 
  {"BRST", timezone_t::BRST}, {"BRT", timezone_t::BRT}, {"BST", timezone_t::BST}, {"BTT", timezone_t::BTT}, {"CDT", timezone_t::CDT}, {"CEST", timezone_t::CEST}, 
  {"CET", timezone_t::CET}, {"CHADT", timezone_t::CHADT}, {"CHAST", timezone_t::CHAST}, {"CHOT", timezone_t::CHOT}, {"CHOST", timezone_t::CHOST}, {"CHST", timezone_t::CHST}, 
  {"CHUT", timezone_t::CHUT}, {"CIST", timezone_t::CIST}, {"CIT", timezone_t::CIT}, {"CKT", timezone_t::CKT}, {"CLST", timezone_t::CLST}, {"CLT", timezone_t::CLT}, 
  {"COT", timezone_t::COT}, {"CST", timezone_t::CST}, {"CT", timezone_t::CT}, {"CVT", timezone_t::CVT}, {"CXT", timezone_t::CXT}, {"ChST", timezone_t::ChST}, 
  {"D", timezone_t::D}, {"DAVT", timezone_t::DAVT}, {"EASST", timezone_t::EASST}, {"EAST", timezone_t::EAST}, {"EAT", timezone_t::EAT}, {"ECT", timezone_t::ECT}, 
  {"EDT", timezone_t::EDT}, {"EEST", timezone_t::EEST}, {"EET", timezone_t::EET}, {"EGST", timezone_t::EGST}, {"EGT", timezone_t::EGT}, {"EST", timezone_t::EST}, 
  {"ET", timezone_t::ET}, {"F", timezone_t::F}, {"FET", timezone_t::FET}, {"FJST", timezone_t::FJST}, {"FJT", timezone_t::FJT}, {"FKST", timezone_t::FKST}, 
  {"FKT", timezone_t::FKT}, {"FNT", timezone_t::FNT}, {"G", timezone_t::G}, {"GALT", timezone_t::GALT}, {"GAMT", timezone_t::GAMT}, {"GET", timezone_t::GET}, 
  {"GFT", timezone_t::GFT}, {"GILT", timezone_t::GILT}, {"GMT", timezone_t::GMT}, {"GST", timezone_t::GST}, {"GYT", timezone_t::GYT}, {"H", timezone_t::H}, 
  {"HAA", timezone_t::HAA}, {"HAC", timezone_t::HAC}, {"HADT", timezone_t::HADT}, {"HAE", timezone_t::HAE}, {"HAP", timezone_t::HAP}, {"HAR", timezone_t::HAR}, 
  {"HAST", timezone_t::HAST}, {"HAT", timezone_t::HAT}, {"HAY", timezone_t::HAY}, {"HKT", timezone_t::HKT}, {"HL", timezone_t::HL}, {"HLV", timezone_t::HLV}, 
  {"HNA", timezone_t::HNA}, {"HNC", timezone_t::HNC}, {"HNE", timezone_t::HNE}, {"HNP", timezone_t::HNP}, {"HNR", timezone_t::HNR}, {"HNT", timezone_t::HNT}, 
  {"HNY", timezone_t::HNY}, {"HOVST", timezone_t::HOVST}, {"HOVT", timezone_t::HOVT}, {"I", timezone_t::I}, {"ICT", timezone_t::ICT}, {"IDT", timezone_t::IDT}, 
  {"IOT", timezone_t::IOT}, {"IRDT", timezone_t::IRDT}, {"IRKT", timezone_t::IRKT}, {"IRST", timezone_t::IRST}, {"IST", timezone_t::IST}, {"JST", timezone_t::JST}, 
  {"K", timezone_t::K}, {"KGT", timezone_t::KGT}, {"KOST", timezone_t::KOST}, {"KRAT", timezone_t::KRAT}, {"KST", timezone_t::KST}, {"KUYT", timezone_t::KUYT}, 
  {"L", timezone_t::L}, {"LHDT", timezone_t::LHDT}, {"LHST", timezone_t::LHST}, {"LINT", timezone_t::LINT}, {"M", timezone_t::M}, {"MAGT", timezone_t::MAGT}, 
  {"MART", timezone_t::MART}, {"MAWT", timezone_t::MAWT}, {"MDT", timezone_t::MDT}, {"MET", timezone_t::MET}, {"MEST", timezone_t::MEST}, {"MHT", timezone_t::MHT}, 
  {"MIST", timezone_t::MIST}, {"MIT", timezone_t::MIT}, {"MMT", timezone_t::MMT}, {"MSK", timezone_t::MSK}, {"MST", timezone_t::MST}, {"MT", timezone_t::MT}, 
  {"MUT", timezone_t::MUT}, {"MVT", timezone_t::MVT}, {"MYT", timezone_t::MYT}, {"N", timezone_t::N}, {"NCT", timezone_t::NCT}, {"NDT", timezone_t::NDT}, 
  {"NFT", timezone_t::NFT}, {"NOVST", timezone_t::NOVST}, {"NOVT", timezone_t::NOVT}, {"NPT", timezone_t::NPT}, {"NRT", timezone_t::NRT}, {"NST", timezone_t::NST},
  {"NT", timezone_t::NT}, {"NUT", timezone_t::NUT}, {"NZDT", timezone_t::NZDT}, {"NZST", timezone_t::NZST}, {"O", timezone_t::O}, {"OMST", timezone_t::OMST}, 
  {"ORAT", timezone_t::ORAT}, {"P", timezone_t::P}, {"PDT", timezone_t::PDT}, {"PET", timezone_t::PET}, {"PETST", timezone_t::PETST}, {"PETT", timezone_t::PETT}, 
  {"PGT", timezone_t::PGT}, {"PHOT", timezone_t::PHOT}, {"PHT", timezone_t::PHT}, {"PKT", timezone_t::PKT}, {"PMDT", timezone_t::PMDT}, {"PMST", timezone_t::PMST}, 
  {"PONT", timezone_t::PONT}, {"PST", timezone_t::PST}, {"PT", timezone_t::PT}, {"PWT", timezone_t::PWT}, {"PYST", timezone_t::PYST}, {"PYT", timezone_t::PYT}, 
  {"Q", timezone_t::Q}, {"QYZT", timezone_t::QYZT}, {"R", timezone_t::R}, {"RET", timezone_t::RET}, {"ROTT", timezone_t::ROTT}, {"S", timezone_t::S}, 
  {"SAMT", timezone_t::SAMT}, {"SAST", timezone_t::SAST}, {"SBT", timezone_t::SBT}, {"SCT", timezone_t::SCT}, {"SDT", timezone_t::SDT}, {"SGT", timezone_t::SGT}, 
  {"SRET", timezone_t::SRET}, {"SRT", timezone_t::SRT}, {"SST", timezone_t::SST}, {"SYOT", timezone_t::SYOT}, {"T", timezone_t::T}, {"TAHT", timezone_t::TAHT}, 
  {"TFT", timezone_t::TFT}, {"TJT", timezone_t::TJT}, {"TKT", timezone_t::TKT}, {"TLT", timezone_t::TLT}, {"TMT", timezone_t::TMT}, {"TOT", timezone_t::TOT}, 
  {"TRT", timezone_t::TRT}, {"TST", timezone_t::TST}, {"U", timezone_t::U}, {"ULAST", timezone_t::ULAST}, {"ULAT", timezone_t::ULAT}, {"UTC", timezone_t::UTC}, 
  {"UYST", timezone_t::UYST}, {"UYT", timezone_t::UYT}, {"UZT", timezone_t::UZT}, {"V", timezone_t::V}, {"VET", timezone_t::VET}, {"VLAT", timezone_t::VLAT}, 
  {"VOLT", timezone_t::VOLT}, {"VOST", timezone_t::VOST}, {"VUT", timezone_t::VUT}, {"W", timezone_t::W}, {"WAKT", timezone_t::WAKT}, {"WARST", timezone_t::WARST}, 
  {"WAST", timezone_t::WAST}, {"WAT", timezone_t::WAT}, {"WEST", timezone_t::WEST}, {"WET", timezone_t::WET}, {"WFT", timezone_t::WFT}, {"WGST", timezone_t::WGST}, 
  {"WGT", timezone_t::WGT}, {"WIB", timezone_t::WIB}, {"WIT", timezone_t::WIT}, {"WITA", timezone_t::WITA}, {"WST", timezone_t::WST}, {"WT", timezone_t::WT}, 
  {"X", timezone_t::X}, {"Y", timezone_t::Y}, {"YAKT", timezone_t::YAKT}, {"YEKT", timezone_t::YEKT}, {"Z", timezone_t::Z}
};

int timezone_offset(char *timezone) {
  auto it = timeZoneMap.find(timezone);
  if (it != timeZoneMap.end()) {
    return (int)(it->second);
  } else {
    return 0;
  }
}