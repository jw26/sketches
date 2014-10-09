#include <Arduino.h>
#define NUM_NAMES 89
static const char PROGMEM string_0[] = "Abbey\0";
static const char PROGMEM string_1[] = "Ada\0";
static const char PROGMEM string_2[] = "Affery\0";
static const char PROGMEM string_3[] = "Agnes\0";
static const char PROGMEM string_4[] = "Alice\0";
static const char PROGMEM string_5[] = "Amy\0";
static const char PROGMEM string_6[] = "Anne\0";
static const char PROGMEM string_7[] = "Annie\0";
static const char PROGMEM string_8[] = "Arabella\0";
static const char PROGMEM string_9[] = "Arethusa\0";
static const char PROGMEM string_10[] = "Barbara\0";
static const char PROGMEM string_11[] = "Bella\0";
static const char PROGMEM string_12[] = "Belle\0";
static const char PROGMEM string_13[] = "Belinda\0";
static const char PROGMEM string_14[] = "Bertha\0";
static const char PROGMEM string_15[] = "Betsy  \0";
static const char PROGMEM string_16[] = "Betty\0";
static const char PROGMEM string_17[] = "Biddy\0";
static const char PROGMEM string_18[] = "Camilla\0";
static const char PROGMEM string_19[] = "Caroline  \0";
static const char PROGMEM string_20[] = "Cecilia  \0";
static const char PROGMEM string_21[] = "Charity  \0";
static const char PROGMEM string_22[] = "Charlotte  \0";
static const char PROGMEM string_23[] = "Clara\0";
static const char PROGMEM string_24[] = "Clarissa\0";
static const char PROGMEM string_25[] = "Clemency\0";
static const char PROGMEM string_26[] = "Cleopatra\0";
static const char PROGMEM string_27[] = "Cornelia\0";
static const char PROGMEM string_28[] = "Dolly\0";
static const char PROGMEM string_29[] = "Dora\0";
static const char PROGMEM string_30[] = "Edith\0";
static const char PROGMEM string_31[] = "Emily\0";
static const char PROGMEM string_32[] = "Emma\0";
static const char PROGMEM string_33[] = "Estella\0";
static const char PROGMEM string_34[] = "Esther\0";
static const char PROGMEM string_35[] = "Fanny\0";
static const char PROGMEM string_36[] = "Flora\0";
static const char PROGMEM string_37[] = "Florence  \0";
static const char PROGMEM string_38[] = "Georgiana\0";
static const char PROGMEM string_39[] = "Grace\0";
static const char PROGMEM string_40[] = "Harriet\0";
static const char PROGMEM string_41[] = "Helena\0";
static const char PROGMEM string_42[] = "Henrietta\0";
static const char PROGMEM string_43[] = "Honoria\0";
static const char PROGMEM string_44[] = "Hortense\0";
static const char PROGMEM string_45[] = "Isabella  Jane\0";
static const char PROGMEM string_46[] = "Janet\0";
static const char PROGMEM string_47[] = "Jenny\0";
static const char PROGMEM string_48[] = "Judy\0";
static const char PROGMEM string_49[] = "Julia\0";
static const char PROGMEM string_50[] = "Kate\0";
static const char PROGMEM string_51[] = "Kitty\0";
static const char PROGMEM string_52[] = "Laura\0";
static const char PROGMEM string_53[] = "Lavinia\0";
static const char PROGMEM string_54[] = "Lillian\0";
static const char PROGMEM string_55[] = "Lizzie\0";
static const char PROGMEM string_56[] = "Louisa  \0";
static const char PROGMEM string_57[] = "Lucie\0";
static const char PROGMEM string_58[] = "Lucretia\0";
static const char PROGMEM string_59[] = "Madeline\0";
static const char PROGMEM string_60[] = "Maggy\0";
static const char PROGMEM string_61[] = "Margaret  \0";
static const char PROGMEM string_62[] = "Marion\0";
static const char PROGMEM string_63[] = "Martha\0";
static const char PROGMEM string_64[] = "Mary\0";
static const char PROGMEM string_65[] = "Matilda\0";
static const char PROGMEM string_66[] = "May\0";
static const char PROGMEM string_67[] = "Mercy  \0";
static const char PROGMEM string_68[] = "Milly\0";
static const char PROGMEM string_69[] = "Molly\0";
static const char PROGMEM string_70[] = "Morleena\0";
static const char PROGMEM string_71[] = "Nancy\0";
static const char PROGMEM string_72[] = "Nelly  \0";
static const char PROGMEM string_73[] = "Ninetta\0";
static const char PROGMEM string_74[] = "Peg\0";
static const char PROGMEM string_75[] = "Pleasant\0";
static const char PROGMEM string_76[] = "Polly\0";
static const char PROGMEM string_77[] = "Rachael\0";
static const char PROGMEM string_78[] = "Rosa\0";
static const char PROGMEM string_79[] = "Rose\0";
static const char PROGMEM string_80[] = "Ruth\0";
static const char PROGMEM string_81[] = "Sarah\0";
static const char PROGMEM string_82[] = "Sarah\0";
static const char PROGMEM string_83[] = "Sophia\0";
static const char PROGMEM string_84[] = "Sophie\0";
static const char PROGMEM string_85[] = "Sophronia\0";
static const char PROGMEM string_86[] = "Sophy\0";
static const char PROGMEM string_87[] = "Susan\0";
static const char PROGMEM string_88[] = "Tilly\0";
static const char PROGMEM string_89[] = "Volumnia\0";
static PGM_P const names[] PROGMEM = {
string_0,
string_1,
string_2,
string_3,
string_4,
string_5,
string_6,
string_7,
string_8,
string_9,
string_10,
string_11,
string_12,
string_13,
string_14,
string_15,
string_16,
string_17,
string_18,
string_19,
string_20,
string_21,
string_22,
string_23,
string_24,
string_25,
string_26,
string_27,
string_28,
string_29,
string_30,
string_31,
string_32,
string_33,
string_34,
string_35,
string_36,
string_37,
string_38,
string_39,
string_40,
string_41,
string_42,
string_43,
string_44,
string_45,
string_46,
string_47,
string_48,
string_49,
string_50,
string_51,
string_52,
string_53,
string_54,
string_55,
string_56,
string_57,
string_58,
string_59,
string_60,
string_61,
string_62,
string_63,
string_64,
string_65,
string_66,
string_67,
string_68,
string_69,
string_70,
string_71,
string_72,
string_73,
string_74,
string_75,
string_76,
string_77,
string_78,
string_79,
string_80,
string_81,
string_82,
string_83,
string_84,
string_85,
string_86,
string_87,
string_88,
string_89,
};
