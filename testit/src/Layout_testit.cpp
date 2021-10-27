#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <Layout.hpp>
#include <iostream>

using ::testing::_;

class Layout_testit : public ::testing::Test {
public:
  float teeFloat(float f) {
    return f;
  }

  float haeFloatMuistipaikasta(std::vector<char> D, uint paikka) {
    assert(!D.empty());
    assert(paikka <= D.size() - sizeof(float));
    return *(float*)(&D[paikka]);
  }
};

TEST_F(Layout_testit, yksiFloat) {
  std::vector<LayoutinElementti> muoto {
    {TYYPPI_FLOAT, 1}
  };
  Layout L(muoto);
  std::string oikeaTulos {"[0, 3]: float\n"};
  ASSERT_EQ(L.haeMuotoStringina(), oikeaTulos);
}

TEST_F(Layout_testit, kaksiFloattia) {
  std::vector<LayoutinElementti> muoto {
    {TYYPPI_FLOAT, 1},
    {TYYPPI_FLOAT, 1}
  };
  Layout L(muoto);
  std::string oikeaTulos {
R"([0, 3]: float
[4, 7]: float
)"};
  ASSERT_EQ(L.haeMuotoStringina(), oikeaTulos);
}

TEST_F(Layout_testit, kaksiFloattiaKerralla) {
  std::vector<LayoutinElementti> muoto {
    {TYYPPI_FLOAT, 2}
  };
  Layout L(muoto);
  std::string oikeaTulos {
R"([0, 3]: float
[4, 7]: float
)"};
  ASSERT_EQ(L.haeMuotoStringina(), oikeaTulos);
}

TEST_F(Layout_testit, floatArray) {
  std::vector<LayoutinElementti> muoto {
    {TYYPPI_ARRAY_FLOAT, 3}
  };
  Layout L(muoto);
  std::string oikeaTulos {
R"([0, 15]: float array [0]
[16, 31]: float array [1]
[32, 47]: float array [2]
)"};
  ASSERT_EQ(L.haeMuotoStringina(), oikeaTulos);
}

TEST_F(Layout_testit, kaksiFloattiaJaFloatArray) {
  std::vector<LayoutinElementti> muoto {
    {TYYPPI_FLOAT, 2},
    {TYYPPI_ARRAY_FLOAT, 3}
  };
  Layout L(muoto);
  std::string oikeaTulos {
R"([0, 3]: float
[4, 7]: float
[16, 31]: float array [0]
[32, 47]: float array [1]
[48, 63]: float array [2]
)"};
  ASSERT_EQ(L.haeMuotoStringina(), oikeaTulos);
}

TEST_F(Layout_testit, maaritteleminenFunktiolla) {
  float f1 {0.0};
  float f2 {1.1};
  float f3 {2.2};
  float f4 {5.14573};
  std::vector<float> F {0.1, 1.2, 2.3};
  Layout L;
  L.maaritteleJasen(f1);
  L.maaritteleJasen(f2);
  L.maaritteleJasen(f3);
  L.maaritteleJasen(f4);
  L.maaritteleJasen(F);
  std::string oikeaTulos {
R"([0, 3]: float
[4, 7]: float
[8, 11]: float
[12, 15]: float
[16, 31]: float array [0]
[32, 47]: float array [1]
[48, 63]: float array [2]
)"};
  ASSERT_EQ(L.haeMuotoStringina(), oikeaTulos);
}

TEST_F(Layout_testit, maaritteleminenFunktiollaValiaikaismuuttujin) {
  Layout L;
  L.maaritteleJasen(teeFloat(0.0) );
  L.maaritteleJasen(float {1.1});
  L.maaritteleJasen(std::vector<float> {0.1, 1.2, 2.3});
  std::string oikeaTulos {
R"([0, 3]: float
[4, 7]: float
[16, 31]: float array [0]
[32, 47]: float array [1]
[48, 63]: float array [2]
)"};
  ASSERT_EQ(L.haeMuotoStringina(), oikeaTulos);
}

/* TODO: määrittele kaikki erityyppiset muuttujat samalla funktiolla tyyliin:
  Layout L {float {0.0}, std::vector<float> {1.1, 2.5} };
*/

TEST_F(Layout_testit, haeFloatit) {
  Layout L;
  L.maaritteleJasen(float{1.5});
  L.maaritteleJasen(float{2.9});
  std::vector<float> F {L.haeFloatit() };
  std::vector<float> oikeaVastaus {1.5, 2.9};
  ASSERT_EQ(F, oikeaVastaus);
}

TEST_F(Layout_testit, haeFloatArrayt) {
  Layout L;
  L.maaritteleJasen(std::vector<float> {1.5, 2.9} );
  L.maaritteleJasen(std::vector<float> {2.9, 2.99, 2.999} );
  std::vector< std::vector<float> > oikeaVastaus {
    {1.5, 2.9},
    {2.9, 2.99, 2.999}
  };
  std::vector< std::vector<float> > F {L.haeFloatArrayt() };
  ASSERT_EQ(F, oikeaVastaus);
}

TEST_F(Layout_testit, haeDatanKoko_KunMaariteltyMaaritteleJasen_funktiolla) {
  Layout L;
  L.maaritteleJasen(float {1.5} );
  L.maaritteleJasen(float {2.9} );
  L.maaritteleJasen(std::vector<float> {2.9, 2.99, 2.999} );
  ASSERT_EQ(L.datanKoko(), 64);
  Layout M;
  M.maaritteleJasen(float{1.0});
  M.maaritteleJasen(float{-5.0});
  ASSERT_EQ(M.datanKoko(), 8);
}


TEST_F(Layout_testit, muodostaData) {
  Layout L;
  L.maaritteleJasen(float {0.01f} );
  L.maaritteleJasen(float {1.105f});
  L.maaritteleJasen(std::vector<float> {0.11f, 1.21f, 2.31f});
  std::vector<char> D {L.muodostaData() };
  ASSERT_EQ(haeFloatMuistipaikasta(D, 0), 0.01f);
  ASSERT_EQ(haeFloatMuistipaikasta(D, 4), 1.105f);
  ASSERT_EQ(haeFloatMuistipaikasta(D, 16), 0.11f);
  ASSERT_EQ(haeFloatMuistipaikasta(D, 32), 1.21f);
  ASSERT_EQ(haeFloatMuistipaikasta(D, 48), 2.31f);
}
