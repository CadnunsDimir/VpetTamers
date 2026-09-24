typedef struct {
    Color color;
    SDL_FRect rect;
} VpetPart;

// ==========================================
// 1. PET_BK_MBARACAJAMON
// ==========================================
const Color BK_MBARACAJAMON_CALDA   = {90, 95, 100, 255};
const Color BK_MBARACAJAMON_PATAS   = {80, 85, 90, 255};
const Color BK_MBARACAJAMON_BARRIGA = {235, 215, 200, 255};
const Color BK_MBARACAJAMON_CORPO   = {100, 105, 110, 255};
const Color BK_MBARACAJAMON_CABECA  = {100, 105, 110, 255};
const Color BK_MBARACAJAMON_ORELHA  = {80, 85, 90, 255};

VpetPart PET_BK_MBARACAJAMON_BATTLE_MODE[] = {
    // 1. Cauda
    { .color = BK_MBARACAJAMON_CALDA,   .rect = { -10.0f,   6.0f,  7.0f, 16.0f } }, // tail_1
    { .color = BK_MBARACAJAMON_CALDA,   .rect = { -14.0f,  -2.0f,  8.0f, 12.0f } }, // tail_2
    { .color = BK_MBARACAJAMON_CALDA,   .rect = { -12.0f,  -8.0f,  7.0f,  8.0f } }, // tail_tip

    // 2. Patas Traseiras
    { .color = BK_MBARACAJAMON_PATAS,   .rect = {  -2.0f,  22.0f,  5.0f, 12.0f } }, // leg_back_l
    { .color = BK_MBARACAJAMON_PATAS,   .rect = {   6.0f,  24.0f,  5.0f, 10.0f } }, // leg_back_r

    // 3. Barriga
    { .color = BK_MBARACAJAMON_BARRIGA, .rect = {   1.0f,  16.0f, 10.0f, 10.0f } }, // belly_light

    // 4. Dorso / Corpo
    { .color = BK_MBARACAJAMON_CORPO,   .rect = {   2.0f,  10.0f, 18.0f, 15.0f } }, // body

    // 5. Patas Dianteiras
    { .color = BK_MBARACAJAMON_PATAS,   .rect = {  12.0f,  18.0f,  4.0f, 10.0f } }, // leg_front_l
    { .color = BK_MBARACAJAMON_PATAS,   .rect = {  17.0f,  15.0f,  4.0f, 10.0f } }, // leg_front_r

    // 6. Cabeça e Bochechas
    { .color = BK_MBARACAJAMON_CABECA,  .rect = {  10.0f,   2.0f, 16.0f, 13.0f } }, // head
    { .color = BK_MBARACAJAMON_CABECA,  .rect = {   8.0f,   6.0f,  4.0f,  6.0f } }, // cheek_l
    { .color = BK_MBARACAJAMON_CABECA,  .rect = {  24.0f,   6.0f,  4.0f,  6.0f } }, // cheek_r

    // 7. Orelhas
    { .color = BK_MBARACAJAMON_ORELHA,  .rect = {  10.0f,  -5.0f,  5.0f,  8.0f } }, // ear_l
    { .color = BK_MBARACAJAMON_ORELHA,  .rect = {  20.0f,  -4.0f,  5.0f,  8.0f } }  // ear_r
};
#define PET_BK_MBARACAJAMON_BATTLE_MODE_COUNT (sizeof(PET_BK_MBARACAJAMON_BATTLE_MODE) / sizeof(PET_BK_MBARACAJAMON_BATTLE_MODE[0]))

// ==========================================
// 2. PET_JAGUAPIRUMON (O único que possui chamas)
// ==========================================
const Color JAGUAPIRUMON_CHAMA_EXTERNA  = {255, 100, 0, 120};
const Color JAGUAPIRUMON_CHAMA_INTERNA  = {255, 200, 0, 160};
const Color JAGUAPIRUMON_PATAS          = {40, 20, 10, 255};
const Color JAGUAPIRUMON_PATA_DIANTEIRA = {50, 25, 12, 255};
const Color JAGUAPIRUMON_GARRAS          = {10, 10, 10, 255};
const Color JAGUAPIRUMON_CORPO           = {45, 22, 12, 255};
const Color JAGUAPIRUMON_ESPINHA         = {25, 12, 6, 255};
const Color JAGUAPIRUMON_CORRENTE        = {180, 180, 190, 255};
const Color JAGUAPIRUMON_CABECA          = {35, 18, 8, 255};
const Color JAGUAPIRUMON_ORELHA          = {20, 10, 5, 255};
const Color JAGUAPIRUMON_DENTES          = {240, 230, 180, 255};
const Color JAGUAPIRUMON_OLHO            = {255, 255, 100, 255};
const Color JAGUAPIRUMON_NUCLEO_OLHO     = {255, 50, 0, 255};
const Color JAGUAPIRUMON_FOGO_MANDIBULA  = {255, 150, 0, 200};

VpetPart PET_JAGUAPIRUMON_BATTLE_MODE[] = {
    // 1. Auras de Chama
    { .color = JAGUAPIRUMON_CHAMA_EXTERNA, .rect = {  -8.0f,  -6.0f, 48.0f, 42.0f } }, // flame_outer
    { .color = JAGUAPIRUMON_CHAMA_INTERNA, .rect = {  -4.0f,  -2.0f, 40.0f, 34.0f } }, // flame_inner

    // 2. Patas e Garras
    { .color = JAGUAPIRUMON_PATAS,          .rect = {  22.0f,  22.0f,  6.0f, 12.0f } }, // leg_back_l
    { .color = JAGUAPIRUMON_PATAS,          .rect = {  16.0f,  24.0f,  6.0f, 12.0f } }, // leg_back_r
    { .color = JAGUAPIRUMON_PATAS,          .rect = {   4.0f,  22.0f,  7.0f, 14.0f } }, // leg_front_l
    { .color = JAGUAPIRUMON_PATA_DIANTEIRA, .rect = {  -2.0f,  24.0f,  8.0f, 14.0f } }, // leg_front_r
    { .color = JAGUAPIRUMON_GARRAS,         .rect = {  -3.0f,  36.0f,  9.0f,  3.0f } }, // claws

    // 3. Corpo e Espinha
    { .color = JAGUAPIRUMON_CORPO,          .rect = {   2.0f,   8.0f, 26.0f, 18.0f } }, // body
    { .color = JAGUAPIRUMON_ESPINHA,        .rect = {   6.0f,   6.0f, 20.0f,  6.0f } }, // spine

    // 4. Corrente
    { .color = JAGUAPIRUMON_CORRENTE,       .rect = {  12.0f,   4.0f,  6.0f,  5.0f } }, // chain_link1
    { .color = JAGUAPIRUMON_CORRENTE,       .rect = {  16.0f,   1.0f,  5.0f,  5.0f } }, // chain_link2

    // 5. Cabeça, Focinho e Orelhas
    { .color = JAGUAPIRUMON_CABECA,         .rect = {  -4.0f,   6.0f, 16.0f, 14.0f } }, // head
    { .color = JAGUAPIRUMON_CABECA,         .rect = { -10.0f,  12.0f, 12.0f, 10.0f } }, // snout
    { .color = JAGUAPIRUMON_ORELHA,         .rect = {   4.0f,  -2.0f,  4.0f, 10.0f } }, // ear1
    { .color = JAGUAPIRUMON_ORELHA,         .rect = {   9.0f,   1.0f,  4.0f,  8.0f } }, // ear2

    // 6. Dentes, Olhos e Fogo
    { .color = JAGUAPIRUMON_DENTES,         .rect = { -10.0f,  20.0f, 10.0f,  3.0f } }, // teeth
    { .color = JAGUAPIRUMON_OLHO,           .rect = {  -1.0f,  10.0f,  4.0f,  3.0f } }, // eye
    { .color = JAGUAPIRUMON_NUCLEO_OLHO,    .rect = {  -1.0f,  11.0f,  2.0f,  2.0f } }, // eye_core
    { .color = JAGUAPIRUMON_FOGO_MANDIBULA, .rect = { -12.0f,  18.0f,  6.0f,  6.0f } }  // jaw_fire
};
#define PET_JAGUAPIRUMON_BATTLE_MODE_COUNT (sizeof(PET_JAGUAPIRUMON_BATTLE_MODE) / sizeof(PET_JAGUAPIRUMON_BATTLE_MODE[0]))

// ==========================================
// 3. PET_RUGUASUMON (Sem chamas)
// ==========================================
const Color RUGUASUMON_CAUDA   = {20, 80, 65, 255};
const Color RUGUASUMON_PATAS   = {230, 190, 40, 255};
const Color RUGUASUMON_PEITO   = {20, 80, 65, 255};
const Color RUGUASUMON_ASA     = {180, 65, 25, 255};
const Color RUGUASUMON_PESCOCO = {240, 180, 30, 255};
const Color RUGUASUMON_CRISTA  = {220, 35, 35, 255};
const Color RUGUASUMON_BARBELA = {220, 35, 35, 255};
const Color RUGUASUMON_BICO    = {245, 190, 20, 255};
const Color RUGUASUMON_OLHO    = {10, 10, 10, 255};

VpetPart PET_RUGUASUMON_BATTLE_MODE[] = {
    // 1. Cauda
    { .color = RUGUASUMON_CAUDA,   .rect = {  18.0f,  -2.0f, 12.0f, 20.0f } }, // tail_main
    { .color = RUGUASUMON_CAUDA,   .rect = {  14.0f,  -8.0f, 14.0f, 10.0f } }, // tail_top

    // 2. Patas
    { .color = RUGUASUMON_PATAS,   .rect = {   6.0f,  28.0f,  3.0f,  8.0f } }, // leg_l
    { .color = RUGUASUMON_PATAS,   .rect = {  14.0f,  28.0f,  3.0f,  8.0f } }, // leg_r

    // 3. Peito
    { .color = RUGUASUMON_PEITO,   .rect = {   2.0f,  18.0f, 14.0f, 12.0f } }, // breast

    // 4. Asa
    { .color = RUGUASUMON_ASA,     .rect = {   8.0f,  14.0f, 14.0f, 12.0f } }, // wing

    // 5. Pescoço e Cabeça
    { .color = RUGUASUMON_PESCOCO, .rect = {   2.0f,   6.0f, 10.0f, 14.0f } }, // neck
    { .color = RUGUASUMON_PESCOCO, .rect = {  -1.0f,   2.0f, 11.0f,  8.0f } }, // head

    // 6. Crista
    { .color = RUGUASUMON_CRISTA,  .rect = {   1.0f,  -4.0f,  4.0f,  7.0f } }, // comb_1
    { .color = RUGUASUMON_CRISTA,  .rect = {   5.0f,  -5.0f,  4.0f,  8.0f } }, // comb_2

    // 7. Barbela
    { .color = RUGUASUMON_BARBELA, .rect = {  -2.0f,  10.0f,  4.0f,  6.0f } }, // wattle

    // 8. Bico
    { .color = RUGUASUMON_BICO,    .rect = {  -6.0f,   4.0f,  6.0f,  4.0f } }, // beak

    // 9. Olho
    { .color = RUGUASUMON_OLHO,    .rect = {   2.0f,   4.0f,  2.0f,  2.0f } }  // eye
};
#define PET_RUGUASUMON_BATTLE_MODE_COUNT (sizeof(PET_RUGUASUMON_BATTLE_MODE) / sizeof(PET_RUGUASUMON_BATTLE_MODE[0]))