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

#ifndef SUBGHZDECODER_NTP_TIMEZONES_H_
#define SUBGHZDECODER_NTP_TIMEZONES_H_

enum class timezone_t {
  A = 3600, ACDT = 37800, ACST = 34200, ACT = -18000, ACWST = 31500, ADT = -10800, 
  AEDT = 39600, AEST = 36000, AFT = 16200, AKDT = -28800, AKST = -32400, ALMT = 21600, 
  AMST = -10800, AMT = -14400, ANAST = 43200, ANAT = 43200, AQTT = 18000, ART = -10800, 
  AST = -14400, AT = -14400, AWDT = 28800, AWST = 28800, AZOST = 0, AZOT = -3600, 
  AZST = 18000, AZT = 14400, AoE = -43200, B = 7200, BNT = 28800, BOT = -14400, 
  BRST = -7200, BRT = -10800, BST = 3600, BTT = 21600, CDT = -18000, CEST = 7200, 
  CET = 3600, CHADT = 49500, CHAST = 45900, CHOT = 28800, CHOST = 32400, CHST = 36000, 
  CHUT = 36000, CIST = -28800, CIT = 28800, CKT = -36000, CLST = -10800, CLT = -14400, 
  COT = -18000, CST = -21600, CT = -21600, CVT = -3600, CXT = 25200, ChST = 36000, 
  D = 14400, DAVT = 25200, EASST = -18000, EAST = -21600, EAT = 10800, ECT = -18000, 
  EDT = -14400, EEST = 10800, EET = 7200, EGST = 0, EGT = -3600, EST = -18000, 
  ET = -18000, F = 18000, FET = 10800, FJST = 46800, FJT = 43200, FKST = -10800, 
  FKT = -14400, FNT = -7200, G = 14400, GALT = -21600, GAMT = -32400, GET = 14400, 
  GFT = -10800, GILT = 43200, GMT = 0, GST = 14400, GYT = -14400, H = 18000, 
  HAA = -10800, HAC = -18000, HADT = -32400, HAE = -14400, HAP = -25200, HAR = -21600, 
  HAST = -36000, HAT = -9000, HAY = -28800, HKT = 28800, HL = 28800, HLV = -14400, 
  HNA = -14400, HNC = -21600, HNE = -18000, HNP = -28800, HNR = -25200, HNT = -12600, 
  HNY = -32400, HOVST = 25200, HOVT = 21600, I = 14400, ICT = 25200, IDT = 10800, 
  IOT = 21600, IRDT = 16200, IRKT = 32400, IRST = 12600, IST = 19800, JST = 32400, 
  K = 18000, KGT = 21600, KOST = 39600, KRAT = 25200, KST = 32400, KUYT = 14400, 
  L = 14400, LHDT = 39600, LHST = 37800, LINT = 50400, M = 18000, MAGT = 43200, 
  MART = -34200, MAWT = 18000, MDT = -21600, MET = 7200, MEST = 7200, MHT = 43200, 
  MIST = 43200, MIT = -34200, MMT = 23400, MSK = 10800, MST = -25200, MT = -25200, 
  MUT = 14400, MVT = 18000, MYT = 28800, N = 21600, NCT = 39600, NDT = -9000, 
  NFT = 41400, NOVST = 25200, NOVT = 21600, NPT = 20700, NRT = 43200, NST = -12600,
  NT = -12600, NUT = -39600, NZDT = 46800, NZST = 43200, O = 21600, OMST = 25200, 
  ORAT = 18000, P = 25200, PDT = -25200, PET = -18000, PETST = 43200, PETT = 43200, 
  PGT = 36000, PHOT = 46800, PHT = 28800, PKT = 18000, PMDT = -7200, PMST = -10800, 
  PONT = 39600, PST = -28800, PT = -28800, PWT = 32400, PYST = -7200, PYT = -10800, 
  Q = 28800, QYZT = 18000, R = 28800, RET = 14400, ROTT = -10800, S = 36000, 
  SAMT = 14400, SAST = 7200, SBT = 39600, SCT = 14400, SDT = -7200, SGT = 28800, 
  SRET = 39600, SRT = -10800, SST = -39600, SYOT = 10800, T = 39600, TAHT = -36000, 
  TFT = 18000, TJT = 18000, TKT = 46800, TLT = 32400, TMT = 18000, TOT = 46800, 
  TRT = 10800, TST = 14400, U = 43200, ULAST = 32400, ULAT = 28800, UTC = 0, 
  UYST = -7200, UYT = -10800, UZT = 18000, V = 39600, VET = -14400, VLAT = 39600, 
  VOLT = 14400, VOST = 21600, VUT = 39600, W = 43200, WAKT = 43200, WARST = -10800, 
  WAST = 7200, WAT = 3600, WEST = 3600, WET = 0, WFT = 43200, WGST = -7200, 
  WGT = -10800, WIB = 25200, WIT = 32400, WITA = 28800, WST = 50400, WT = 0, 
  X = 46800, Y = 50400, YAKT = 36000, YEKT = 18000, Z = 0
};

int timezone_offset(char *timezone);

#endif /* SUBGHZDECODER_NTP_TIMEZONES_H_ */