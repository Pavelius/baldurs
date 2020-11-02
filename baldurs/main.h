#include "archive.h"
#include "converter.h"
#include "crt.h"
#include "draw.h"
#include "io.h"
#include "map.h"
#include "resources.h"
#include "screenshoot.h"
#include "stringbuilder.h"

#ifdef _DEBUG
#define msdbg(text, ...) draw::mslog(text, __VA_ARGS__)
#else
#define msdbg(text, ...);
#endif // _DEBUG

const int CP = 1;
const int SP = 10;
const int GP = 100;
const int PP = 1000;

enum geography_s : unsigned char {
	North, East, South, West
};
enum school_s : unsigned char {
	Abjuration, Conjuration, Divination, Enchantment,
	Evocation, Illusion, Necromancy, Transmutation,
};
enum school_type_s : unsigned char {
	Calling, Creation, HealingConjuration, Summoning,
	Charm, Compulsion,
	Figmental, Glamour, Phantasmal, Shadowing,
};
enum spell_s : unsigned short {
	// Spells level 1
	ArmorOfFaith, Bane, Bless, Command,
	CureLightWounds, Doom, Entangle, FaerieFire, FlameStrike,
	FrostFingers, GlyphOfWarding, HoldPerson, InflictLightWounds, MagicStone,
	ProtectionFromEvil, RemoveFear, RemoveParalysis, Sanctuary, Shillelagh,
	Sunscorch, Antichickenator, BurningHands, CharmPerson, ChillTouch,
	ChromaticOrb, ColorSpray, DispelMagic, EagleSSplendor, GenericAbjuration,
	Grease, IceDagger, Identify, LarlochSMinorDrain, MageArmor,
	MagicMissile, MinorMirrorImage, ProtectionFromPetrification, Shield, ShockingGrasp,
	Sleep, Spook, SummonMonsterI,
	FirstSpell = ArmorOfFaith, LastSpell = SummonMonsterI,
};
enum ability_s : unsigned char {
	Strenght, Dexterity, Constitution, Intellegence, Wisdow, Charisma,
	// Addition abilities
	ArmorClass, Attack, Damage, MeleeAttack, MeleeDamage, RangeAttack, RangeDamage, DeflectCritical,
	Fortitude, Reflexes, Will,
	HitPoints, Movement,
	LastAbility = Movement
};
enum class_s : unsigned char {
	// Classes
	Commoner,
	Adept, Aristocrat, Beast, MonsterRace, Undead, Warrior,
	Barbarian, Bard, Cleric, Druid, Fighter, Monk, Paladin, Ranger, Rogue, Sorcerer, Wizard,
	FirstClass = Barbarian, LastClass = Wizard,
};
enum skill_s : unsigned char {
	NoSkill,
	Appraise, Balance, Bluff, Climb, Concentration,
	CraftAlchemy, CraftArmor, CraftBow, CraftDevice, CraftWeapon,
	DecipherScript, Diplomacy, DisableDevice, Disguise,
	EscapeArtist, Forgery, GatherInformation, HandleAnimal, Heal,
	Hide, Intimidate, Jump,
	KnowledgeArcana, KnowledgeHistory, KnowledgeNature, KnowledgeNobility, KnowledgeReligion,
	Listen, MoveSilently, OpenLock, Perform, Ride,
	Search, SenseMotive, SleightOfHand, SpeakLanguage, Spellcraft,
	Spot, Survival, Swim, Tumble, UseMagicDevice,
	UseRope,
	FirstSkill = Appraise, LastSkill = UseRope,
};
enum tag_s : unsigned char {
	ArmorCheck, Balanced, Precise, Deadly, Light, Trained, TwoHanded,
};
enum feat_s : unsigned char {
	Alertness,
	ArmorProfeciencyLight, ArmorProfeciencyMedium, ArmorProfeciencyHeavy,
	BlindFight,
	Cleave, CombatCasting, CombatExpertise, CombatReflexes,
	DeadlyPercision, DeflectsArrows, Dodge, Endurance,
	FarShoot,
	GreateFortitude,
	ImprovedCritical, ImprovedDisarm, ImprovedFeint, ImprovedInitiative, ImprovedOverrun, ImprovedSunder, ImprovedTrip, ImprovedUnarmedStrike,
	IronWill,
	Leadership, LightingReflexes,
	Mobiliy, Multiattack,
	PointBlankShoot, PowerAttack, PreciseShoot,
	RapidShoot, RapidReload,
	ShieldProfeciency,
	ShortOnTheRun,
	SpellFocus,
	SpringAttack, StunningFist,
	Toughness, Track, TwoWeaponFighting,
	FocusAxes, FocusDaggers,
	FocusMaces, FocusPolearm, FocusShooting, FocusSwords,
	WhirlwindAttack,
	// Владение оружием
	ProficiencyAxe, ProficiencyClub, ProficiencyCrossbow,
	ProficiencyDagger, ProficiencyGreatweapon, ProficiencyHeavyCrossbow,
	ProficiencyMace, ProficiencySimple, ProficiencySpear,
	ProficiencyLongbow, ProficiencyLongsword,
	ProficiencyScimitar, ProficiencyShortbow, ProficiencyShortsword,
	ProficiencyBastardsword, ProficiencyCatana, ProficiencyWaraxe,
	// Специальные особенности
	FastMovement, Illiteracy,
	// Особенности расы
	HateGoblinoids, Infravision, ImmunityToSleepSpell, FindSecretDoors, Lucky, PreciseThrower,
	SaveBonusVsEnchantment, SaveBonusVsFear, SaveBonusVsIllusion, SaveBonusVsPoison, SaveBonusVsSpell, Stability, Stonecunning,
	FirstFeat = Alertness, LastFeat = Stonecunning,
};
enum race_s : unsigned char {
	Dwarf, Elf, Gnome, HalfElf, HalfOrc, Halfling, Human,
	Goblinoid, Giant,
	FirstRace = Dwarf, LastRace = Giant,
};
enum gender_s : unsigned char {
	NoGender,
	Male, Female,
};
enum alignment_s : unsigned char {
	LawfulGood, NeutralGood, ChaoticGood, LawfulNeutral, TrueNeutral, ChaoticNeutral, LawfulEvil, NeutralEvil, ChaoticEvil,
};
enum diety_s : unsigned char {
	GodBane, GodChauntea, GodIllmater, GodGruumsh, GodHelm,
	GodKelemvor, GodKossuth, GodLathander, GodLolth, GodMask,
	GodMystra, GodMoradin, GodOghma, GodShar, GodTempus,
	GodTymora, GodTyr, GodUmberly,
};
enum item_s : unsigned char {
	NoItem,
	Club, Hammer, Mace, Spear, Staff, CrossbowLight, CrossbowHeavy, Sling,
	BattleAxe, Dagger, Greataxe, Halberd, HandAxe,
	Scimitar, Longsword, Shortsword, Greatsword, Rapier, Shortbow, Longbow,
	Waraxe, BastardSword, Katana,
	// Items (armor)
	LeatherArmor, StuddedLeatherArmor, ChainShirt,
	ScaleMail, ChainMail, Breastplate,
	SplintMail, BandedMail, Plate,
	ShieldSmall, ShieldLarge, ShieldTower,
	Helm,
	Arrow,
	CooperPieces, SilverPieces, GoldPieces, PlatinumPieces,
	BlueQuarz,
	CarvedStone,
	LastItem = CarvedStone,
};
enum animate_s : unsigned char;
enum slot_s : unsigned char {
	Backpack, LastBackpack = Backpack + 23,
	Head, Neck, Body, Rear, LeftFinger, RightFinger, Hands, Gridle, Legs,
	QuickWeapon, QuickOffhand, LastQuickWeapon = QuickWeapon + 4 * 2,
	Quiver, LastQuiver = Quiver + 2,
	QuickItem, LastQuickItem = QuickItem + 2,
};
enum state_s : unsigned char {
	Stunned, ReadyToBattle,
};
enum action_s : unsigned char {
	ActionGuard, ActionTurnUndead, ActionCast, ActionAttack, ActionUseItem, ActionPerform, ActionStop,
};
enum formation_s : unsigned char {
	NoFormation,
	FormationT, FormationGather, Formation4and2, Formation3by2, FormationProtect,
};
enum monster_s : unsigned char {
	Goblin, LargeSkeleton,
	FirstMonster = Goblin, LastMonster = LargeSkeleton,
};
enum special_s : unsigned char {
	NoSpecial,
	OfBaneOrc,
	OfFlaming, OfFlamingBurst, OfFrost, OfIcyBurst, OfShock, OfGhostTouch,
	OfHoly, OfKeen, OfShockingBurst, OfSpeed, OfThundering, OfUnholy, OfWounding, OfVorpal,
};
enum area_flag_s {
	AreaVisible = 1,
	AreaVisibleFromAdjanced = 2,
	AreaReachable = 4,
	AlreadyVisited = 8,
};
enum sprite_type_s : unsigned char {
	CID1, MID1,
};
enum color_indexes {
	HairBlack, HairLightBrown, HairDarkBrown, HairBlonde, HairRed, HairLightGrey, HairDarkGray, HairLightGreen,
	SkinEbony, SkinDarkBorwn, SkinOlive, SkinRed, SkinPink, SkinWhite, SkinGrey, SkinLightGreen,
	SkinYellow, SkinLightBlue, SkinMedBlue, SkinMedRed, SkinMedGreen,
	LeatherBlack, LeatherLightBrown, LeatherDarkBrown,
	MetalBronze, MetalGold, MetalCooper, MetalSilver, MetalAluminum, MetalObsidian, MetalIron,
	MetalBlue, MetalGreen, MetalRed, MetalRainbow1, MetalRainbow2,
	ClothLightOlive, ClothDarkOlive, ClothLightBrown1, ClothDarkBrown1, ClothLightBrown2, ClothDarkBrown2,
	ClothLightKaiki, ClothDarkKaiki, ClothLightMagenta, ClothDarkMagenta, ClothLightRed, ClothDarkRed,
	ClothLightOrange, ClothDarkOrange, ClothLightYellow, ClothDarkYellow,
	ClothLightGreen, ClothMedGreen, ClothDarkGreen, ClothLightAqua, ClothDarkAqua,
	ClothLightBlue, ClothDarkBlue, ClothLightIndigo, ClothDarkIndigo,
	ClothLightViolet, ClothDarkViolet, ClothWhite, ClothLightGrey, ClothDarkGrey, ClothBlack,
	FireNormal, FireBlue, FireGreen, FleshRendered, IceColor, StoneColor, AcidColor,
	WhiteColor, BlackColor, RedColor, GreenColor, BlueColor,
	HairWhite, HairOrange, HairGold, HairLightBlue,
	SkinIndigo, SkinHardy, SkinRugged, SkinMerman, SkinNubian, SkinSunburn, SkinPale, SkinOriental,
	LeatherDarkBrown2, LeatherLightBrown2,
	LeatherUmber, LeatherSienna, LeatherLightTan,
	MetalSteel, MetalMithral, MetalAntique, MetalDarkGold, MetalBurnished, MetalCherry,
	MEtalSlate, MetalEmerald, MetalViolet,
	SkinAlabaster, SkinLightGrey, SkinDarkGrey, SkinPinkpale, SkinCooper,
	HairSilver, HairCooper,
	SkinGrayGreen, SkinBrownGreen, SkinRichbrown,
};
namespace res {
enum tokens {
	NONE,
	CURSORS, CURSARW, COLGRAD, COLOR,
	GACTN,
	GBTNBFRM, GBTNJBTN, GBTNLRG, GBTNMED, GBTNMED2, GBTNMINS, GBTNPLUS, GBTNOPT1, GBTNOPT2,
	GBTNSPB1, GBTNSPB2, GBTNSPB3,
	GBTNPOR, GUICONT, GUIMAP, GUIMAPWC, GUIREC, GBTNRECB, GBTNSCRL, GBTNSTD,
	GCOMM, GCOMMBTN, FLAG1,
	INVBUT2, INVBUT3, ITEMS, GROUND, CONTAINER, SCROLLS, FORM,
	CDMB1, CDMB2, CDMB3, CDMC4, CDMF4, CDMT1, CDMW1, CDMW2, CDMW3, CDMW4,
	CEFB1, CEFB2, CEFB3, CEFC4, CEFF4, CEFT1, CEFW1, CEFW2, CEFW3, CEFW4,
	CEMB1, CEMB2, CEMB3, CEMC4, CEMF4, CEMT1, CEMW1, CEMW2, CEMW3, CEMW4,
	CHFB1, CHFB2, CHFB3, CHFC4, CHFF4, CHFT1, CHFW1, CHFW2, CHFW3, CHFW4, CHFM1,
	CHMB1, CHMB2, CHMB3, CHMC4, CHMF4, CHMT1, CHMW1, CHMW2, CHMW3, CHMW4, CHMM1,
	CIFB1, CIFB2, CIFB3, CIFC4, CIFF4, CIFT1,
	CIMB1, CIMB2, CIMB3, CIMC4, CIMF4, CIMT1,
	GUIA, GUIBTACT, GUIBTBUT,
	GUICGB, GUICHP, GUIHELP, GUIFEAT, GUIINV, GIITMH08, GUIHSB, GUICARBB,
	GUIPFC, GUIRSPOR, GUISEX, GUISPL, GUICNAME, GUIERR,
	GUIRLVL, GMPMCHRB, GCGPARTY, GBTNLRG2,
	GUIJRLN, GOPT, GUIMOVB, STONEOPT, GUISRSVB, GUISRRQB,
	CGEAR,
	LOAD, LOCATER,
	PORTL, PORTS,
	SPELLS, SPLBUT, START, STON, STONSLOT, WMAP,
	// Оружие персонажей
	WQSAX, WQNAX, WQMAX, WQLAX,
	WQSBW, WQNBW, WQMBW, WQLBW,
	WQSCB, WQNCB, WQMCB, WQLCB,
	WQSD1, WQND1, WQMD1, WQLD1,
	WQSD2, WQND2, WQMD2, WQLD2,
	WQSD3, WQND3, WQMD3, WQLD3,
	WQSD4, WQND4, WQMD4, WQLD4,
	WQSDD, WQNDD, WQMDD, WQLDD,
	WQSFL, WQNFL, WQMFL, WQLFL,
	WQSH0, WQNH0, WQMH0, WQLH0,
	WQSH1, WQNH1, WQMH1, WQLH1,
	WQSH2, WQNH2, WQMH2, WQLH2,
	WQSH3, WQNH3, WQMH3, WQLH3,
	WQSH4, WQNH4, WQMH4, WQLH4,
	WQSH5, WQNH5, WQMH5, WQLH5,
	WQSHB, WQNHB, WQMHB, WQLHB,
	WQSMC, WQNMC, WQMMC, WQLMC,
	WQSQS, WQNQS, WQMQS, WQLQS,
	WQSS1, WQNS1, WQMS1, WQLS1,
	WQSS2, WQNS2, WQMS2, WQLS2,
	WQSS3, WQNS3, WQMS3, WQLS3,
	WQSSC, WQNSC, WQMSC, WQLSC,
	WQSSL, WQNSL, WQMSL, WQLSL,
	WQSSP, WQNSP, WQMSP, WQLSP,
	WQSSS, WQNSS, WQMSS, WQLSS,
	WQSWH, WQNWH, WQMWH, WQLWH,
	// Метание стреляние
	ARARROW,
	// Монстры
	MGO1, MSKA, MSKAA,
	Count
};
}
enum region_type_s {
	RegionTriger, RegionInfo, RegionTravel
};
enum button_s {
	ButtonChecked, ButtonNormal, ButtonPressed, ButtonDisabled
};
enum reaction_s : unsigned char {
	Undifferent, Friendly, Helpful, Unfriendly, Hostile,
};
enum damage_s : unsigned char {
	Bludgeon, Slashing, Pierce,
	Acid, Cold, Fire, Lighting, Sound,
};
enum tile_s : unsigned char {
	ObstacleImpassableBlack, Sand, WoodGreen, WoodBrown,
	StoneDry, Grass, WaterTurquose, Stone,
	ObstacleImpassableGrey, WoorRed, Wall, WaterYellow,
	WaterImpassable, RoofImpassable, WorldExit, GrassHardMove,
};
enum variant_s : unsigned char {
	NoVariant,
	Ability, Alignment, Apearance, Class, Container, Creature,
	Diety, Door, Gender, Feat, Item, ItemCont, ItemGround, Monster, Name,
	Position, Race, Reaction, Region, School, Skill, Spell,
	Finish, Variant,
};
enum magic_s : unsigned char {
	Mundane, Minor, Medium, Major,
};
enum duration_s : unsigned char {
	Instant,
	Round, Round1p,
	Minute, Minute1p,
	Hour, Hour1p,
	Day,
	Permanent,
};
enum range_s : unsigned char {
	Touch, Reach,
	Range10,
	Range20,
	Range30,
	Range50,
	Range60,
	Range70,
	Range80,
	Range100,
	Range120,
};
class creature;
class item;
struct container;
struct door;
struct itemground;
struct monsteri;
struct region;
union variant {
	struct {
		variant_s			type;
		unsigned char		count;
		unsigned short		value;
	};
	int						integer;
	constexpr variant() : integer(0) {}
	constexpr variant(variant_s type, unsigned short value) : type(type), value(value), count(0) {}
	constexpr variant(ability_s v) : variant(Ability, v) {}
	constexpr variant(alignment_s v) : variant(Alignment, v) {}
	constexpr variant(class_s v) : variant(Class, v) {}
	constexpr variant(gender_s v) : variant(Gender, v) {}
	constexpr variant(feat_s v) : variant(Feat, v) {}
	constexpr variant(race_s v) : variant(Race, v) {}
	constexpr variant(reaction_s v) : variant(Reaction, v) {}
	constexpr variant(skill_s v) : variant(Skill, v) {}
	constexpr variant(spell_s v) : variant(Spell, v) {}
	constexpr variant(item_s v) : variant(Item, v) {}
	constexpr variant(variant_s v) : variant(Variant, v) {}
	constexpr variant(const point& v) : integer(Position | ((v.y & 0xFFF) << 20) | ((v.x & 0xFFF) << 8)) {}
	constexpr variant(int v) : integer(v) {}
	variant(const void* v);
	constexpr operator int() const { return integer; }
	constexpr explicit operator bool() const { return integer != 0; }
	constexpr bool operator==(const variant& e) const { return integer == e.integer; }
	constexpr bool operator!=(const variant& e) const { return integer != e.integer; }
	void					addinfo(stringbuilder& sb) const;
	void					clear() { integer = 0; }
	void*					getpointer(variant_s t) const;
	creature*				getcreature() const { return (creature*)getpointer(Creature); }
	container*				getcontainer() const { return (container*)getpointer(Container); }
	door*					getdoor() const { return (door*)getpointer(Door); }
	itemground*				getitemground() const { return (itemground*)getpointer(ItemGround); }
	monsteri*				getmonster() const { return (monsteri*)getpointer(Monster); }
	region*					getregion() const { return (region*)getpointer(Region); }
	point					getposition() const { return {(short)((integer >> 8) & 0xFFF), (short)((integer >> 20) & 0xFFF)}; }
};
struct varianta : adat<variant, 512> {
	void					addu(variant v1, variant v2);
	void					addu(variant v1, variant v2, creature& e);
	void					creatures(variant v1, bool ispresent);
	void					match(const creature& e, bool ispresent = true);
	void					select(const variant v1, const variant v2);
	void					select(const variant v1, const variant v2, const creature& player, bool required_feats = true);
	void					sort();
	void					shuffle();
};
template <class T, unsigned N, class DT = char>
struct aset {
	struct element {
		T					key;
		DT					value;
	};
	DT						data[N + 1];
	constexpr aset() : data() {}
	constexpr aset(const std::initializer_list<element>& source) {
		for(auto& e : source)
			data[e.key] = e.value;
	}
	constexpr DT& operator[](unsigned i) { return data[i]; }
};
typedef void(*fnitem)();
typedef cflags<tag_s>		taga;
typedef cflags<feat_s>		feata;
typedef aset<ability_s, LastAbility> abilitya;
typedef aset<skill_s, LastSkill> skilla;
typedef aset<class_s, LastClass> classa;
struct rolli {
	char					dc, bonus, rolled, result;
	bool					iscritical(int modifier) const { return rolled >= (20 - modifier); }
};
struct damagei {
	const char*				id;
	const char*				name;
};
struct reactioni {
	const char*				id;
	const char*				name;
};
struct durationi {
	const char*				id;
	unsigned				rounds;
	unsigned				rounds_increment;
	unsigned				per_levels;
	const char*				name;
	unsigned				getrounds(int level) const;
};
struct rangei {
	const char*				id;
	unsigned				feets;
	int						get(int level) const;
	static int				getsquare(int value) { return value * 2 / 5; }
};
struct dicei {
	damage_s				type;
	unsigned char			c, d;
	char					b;
	constexpr explicit operator bool() const { return c != 0; }
	int						roll() const;
};
struct attacki : rolli {
	dicei					damage;
	char					critical, multiplier;
	short					range;
	item*					weapon;
	item*					ammunition;
	constexpr explicit operator bool() const { return damage.c != 0; }
};
struct itemi {
	struct animationi {
		const char*			avatar;
		const char*			ground;
		res::tokens			wear;
		res::tokens			thrown;
	};
	struct poweri {
		magic_s				rate;
		char				bonus;
		const char*			name;
		variant				power;
		const char*			text;
		const int			getweight() const;
	};
	struct combati {
		dicei				damage;
		short				ac;
		range_s				range;
		short				deflect_critical;
	};
	const char*				id;
	animationi				images;
	slot_s					slot;
	feat_s					feat[2];
	taga					flags;
	combati					ai;
	unsigned char			count;
	int						weight;
	int						cost; // Цена в золотых монетах
	aref<poweri>			power;
	const char*				name;
	const char*				text;
};
class item {
	item_s					type;
	unsigned char			effect;
	unsigned char			count;
	struct {
		unsigned char		identified : 1;
		unsigned char		stolen : 1;
	};
public:
	constexpr explicit operator bool() const { return type != NoItem; }
	constexpr operator item_s() const { return type; }
	constexpr item(item_s t = NoItem) : type(t), effect(0), count(0), identified(1), stolen(0) {}
	void					addinfo(stringbuilder& sb) const;
	void					apply(attacki& a) const;
	void					clear();
	void					equip(const item& it);
	int						getac() const;
	int						getarmorindex() const;
	item_s					getammunition() const;
	static res::tokens		getanwear(int type);
	res::tokens				getanthrown() const;
	int						getbonus() const;
	int						getcost() const;
	int						getcount() const;
	feat_s					getfeat() const;
	static const char*		getfname(int type);
	static const char*		getfgname(int type);
	int						getframe() const;
	constexpr itemi&		geti() const { return bsdata<itemi>::elements[type]; }
	const char*				getname() const { return geti().name; }
	int						getportrait() const { return type * 2; }
	int						getdragportrait() const { return type * 2 + 1; }
	creature*				getowner() const;
	item_s					gettype() const { return type; }
	slot_s					getwear() const;
	constexpr bool			is(feat_s v) const { return geti().feat[0] == v || geti().feat[1] == v; }
	bool					is(slot_s v) const;
	bool					is(tag_s v) const { return geti().flags.is(v); }
	bool					isbow() const;
	bool					islight() const { return geti().slot == QuickOffhand && !isshield(); }
	bool					isknown() const { return identified != 0; }
	bool					isranged() const;
	bool					isreach() const;
	bool					isshield() const { return geti().feat[0] == ShieldProfeciency; }
	bool					isthrown() const;
	bool					isxbow() const;
	void					remove();
	void					setcount(int value);
};
struct abilityi {
	const char*				id;
	ability_s				base;
	cflags<class_s>			classes;
	const char*				name;
	const char*				text;
	void					addinfo(stringbuilder& sb) const;
};
struct genstepi {
	variant_s				step;
	const char*				name;
	variant					from, to;
};
struct genderi {
	const char*				id;
	const char*				name;
	const char*				text;
};
struct alignmenti {
	const char*				id;
	const char*				name;
	const char*				text;
};
struct animatei {
	const char*				name;
	bool					disable_overlay;
	bool					ready;
};
struct varianti {
	const char*				id;
	const char*				name;
	variant					manual[2];
	array*					source;
	unsigned				locale[2];
	unsigned				special[4];
	const char*				text;
};
struct drawable {
	virtual int				getcursor() const { return 0; } // Get cursor index when over this drawable
	virtual unsigned		getfps() const { return 20; }
	virtual point			getposition() const = 0;
	virtual rect			getrect() const = 0;
	virtual int				getzorder() const { return 0; } // Priority for z-order sortering (lesser was be most visible). If there is two drawable in same position.
	virtual bool			hittest(point position) const { return false; }
	virtual bool			isvisible() const { return true; }
	virtual bool			isvisibleactive() const { return false; } // Drawable visible only when active.
	virtual void			painting(point position) const = 0; // How to paint drawable.
	virtual void			update() {}
};
struct ctext {
	int						origin;
	int						maximum;
	int						increment;
	res::tokens				bar;
	const char*				cache_text;
	int						cache_height;
	int						cache_origin;
	int						scroll_frame;
	//
	ctext();
	void					cashing(const char* text, int width);
	virtual void			prerender();
	void					reset();
};
struct clist : ctext {
	virtual void			row(rect rc, int n) = 0;
	virtual int				getmaximum() const { return maximum; }
};
struct citem : clist {
	item*					data[10];
	int						maximum_items;
	int						mx, my;
	citem(int mx, int my) : maximum_items(0), mx(mx), my(my) {}
	void					row(rect rc, int n) override {}
	item*					get(int index) const;
	int						getcount() const { return mx * my; }
	void					update(creature* player, int item_in_line);
	void					update(container* object, int item_in_line);
	void					update(short unsigned index, int item_in_line);
	void					view(creature* player, int x, int y, int dx, int dy, const rect& rcs, fnitem item_proc);
};
struct cursorset {
	cursorset(res::tokens r = res::CURSORS, int f = 267, bool single = false);
	~cursorset();
	static void				set(res::tokens rid = res::CURSORS, int frame = 267, bool single = false);
	static void				setblock() { set(res::CURSORS, 6); }
	static res::tokens		getres();
private:
	res::tokens				rid;
	int						frame;
	bool					single;
};
struct coloration {
	unsigned char			skin;
	unsigned char			hair;
	unsigned char			minor;
	unsigned char			major;
	unsigned char			metal;
	unsigned char			leather;
	unsigned char			armor;
	//
	explicit operator bool() const { return skin == hair == minor == major == 0; }
	constexpr coloration() : skin(0), hair(0), minor(0), major(0), armor(/*28*/MetalSteel), metal(MetalIron), leather(23) {}
	void					set(short unsigned portrait);
	void					upload(color* col) const;
};
struct door_tile {
	short unsigned			index; // index = y*64 + x;
	short unsigned			open; // new tile index
	short unsigned			closed; // new tile index
};
struct hotkey {
	void(*proc)();
	unsigned				key;
	const char*				name;
	explicit operator bool() const { return proc != 0; }
};
struct floattext : drawable {
	const char*				text;
	unsigned				stamp;
	rect					box;
	constexpr floattext() : box(), text(0), stamp(0) {}
	explicit operator bool() const { return text != 0; }
	void					clear();
	bool					isvisible() const override { return stamp && text; }
	point					getposition() const override { return{(short)(box.x1 + box.width() / 2), (short)box.y2}; }
	int						getzorder() const override { return 128; }
	rect					getrect() const override { return{box.x1 - 4, box.y1 - 4, box.x2 + 4, box.y2 + 4}; }
	void					painting(point offset) const override;
	void					update() override;
};
struct animation : drawable, point {
	struct info {
		const sprite*		source;
		int					frame;
		unsigned			flags;
		unsigned char		transparent;
		explicit operator bool() const { return source != 0; }
	};
	unsigned				schedule;
	char					rsname[9], rsname_pallette[9];
	unsigned short			circle, frame, start_frame;
	unsigned				flags;
	unsigned short			height;
	unsigned char			transparency;
	unsigned char			chance_loop;
	unsigned char			skip_cycles;
	constexpr animation() : point(), schedule(0), circle(0), frame(0), start_frame(0),
		flags(0), height(0), transparency(0), chance_loop(0), skip_cycles(0),
		rsname(), rsname_pallette() {
	}
	void					getinfo(info& e) const;
	rect					getrect() const override;
	point					getposition() const override { return *this; }
	int						getzorder() const override { return -height - 256; }
	bool					is(unsigned value) const { return (flags & value) != 0; }
	virtual bool			isvisible() const override;
	void					painting(point screen) const override;
};
struct moveable : drawable {
	moveable() = default;
	moveable(point start, point finish, res::tokens id, unsigned feets_per_second = 30);
	explicit operator bool() const { return avatar != res::NONE; }
	void* operator new(unsigned size);
	bool					isvisible() const override { return avatar != res::NONE; }
	point					getposition() const override { return position; }
	rect					getrect() const override { return {position.x - 16, position.y - 16, position.x + 16, position.y + 16}; }
	void					painting(point screen) const override;
	void					set(const coloration& v) { colors = v; use_colors = true; }
	void					update();
	void					wait();
private:
	point					position, start, finish;
	res::tokens				avatar;
	char					orientation;
	unsigned				time_start, time_finish;
	coloration				colors;
	bool					use_colors;
};
struct selectable : drawable {
	virtual aref<point>		getpoints() const = 0;
	point					getposition() const override;
	bool					hittest(point hittest) const;
	void					painting(point screen) const override;
};
struct itemdrag {
	item*					source;
	item*					target;
	item					value;
	slot_s					target_slot;
};
struct itemground : item, drawable {
	short unsigned			index;
	virtual point			getposition() const;
	virtual rect			getrect() const override;
	virtual int				getzorder() const override;
	virtual bool			hittest(point position) const override;
	virtual void			painting(point position) const;
};
struct varset {
	variant_s				type;
	union {
		aref<spell_s>		spells;
		aref<skill_s>		skills;
	};
	constexpr varset() : type(NoVariant), spells() {}
	constexpr varset(const aref<spell_s>& v) : type(Spell), spells(v) {}
	constexpr varset(const aref<skill_s>& v) : type(Skill), skills(v) {}
};
struct dietyi {
	const char*				id;
	const char*				name;
	const char*				text;
};
struct feati {
	const char*				id;
	char					ability[6];
	std::initializer_list<feat_s> prerequisits;
	char					base_attack;
	char					character_level;
	std::initializer_list<feat_s> prerequisits_oneof;
	const char*				name;
	const char*				text;
	const char*				normal;
	const char*				benefit;
	void					addhead(stringbuilder& sb, const char* prefix = 0) const;
	void					addinfo(stringbuilder& sb) const;
};
struct spelli {
	struct duration_info {
		unsigned char		levels;
		unsigned char		rounds;
	};
	const char*				id;
	char					rsname[10];
	school_s				school;
	classa					levels;
	range_s					range;
	duration_s				duration;
	const char*				name;
	const char*				text;
	void					addinfo(stringbuilder& sb) const;
};
struct schooli {
	const char*				id;
	const char*				name;
	const char*				text;
};
struct portraiti {
	const char*				name;
	unsigned char			skin;
	unsigned char			hair;
	unsigned char			major;
	unsigned char			minor;
	gender_s				gender;
	std::initializer_list<race_s> races;
	std::initializer_list<class_s> perks;
	bool					is(gender_s id) const;
	bool					is(race_s id) const;
	bool					is(class_s id) const;
	static aref<portraiti> getelements();
};
struct racei {
	const char*				id;
	abilitya				abilities;
	class_s					favorite;
	skilla					skills;
	std::initializer_list<feat_s> feats;
	char					quick_learn; // Human's ability additional skills nad feats at start of game
	const char*				name;
	const char*				text;
	void					addinfo(stringbuilder& sb) const;
};
struct classi {
	struct slot_info {
		char				minimal;
		aref<char>			progress;
	};
	const char*				id;
	char					hd;
	char					skill_points;
	slot_info*				spells;
	ability_s				spell_ability;
	aref<skill_s>			class_skills;
	aref<feat_s>			weapon_proficiency;
	aref<feat_s>			armor_proficiency;
	std::initializer_list<alignment_s> alignment_restrict;
	const char*				name;
	const char*				text;
	bool					isclass(skill_s v) const;
	void					addinfo(stringbuilder& sb) const;
};
struct skilli {
	const char*				id;
	ability_s				ability;
	skill_s					synergy[3];
	taga					flags;
	const char*				name;
	const char*				text;
	void					addinfo(stringbuilder& sb) const;
};
struct entrance {
	char					name[32];
	unsigned char			orientation;
	point					position;
};
struct region : selectable {
	region_type_s			type;
	const char*				name;
	point					launch;
	point					use;
	rect					box;
	char					move_to_area[8];
	char					move_to_entrance[32];
	aref<point>				points;
	rect					getrect() const override { return box; }
	int						getcursor() const override;
	aref<point>				getpoints() const override { return points; }
	virtual bool			isvisible() const { return type != RegionTriger; }
	void					painting(point screen) const {}
};
struct container : selectable {
	enum type_s : unsigned char {
		None,
		Bag, Chest, Drawer, Pile, Table, Shelf, Altar, Nonvisible,
		Spellbook, Body, Barrel, Crate
	};
	type_s				type;
	char				name[32];
	point				launch;
	rect				box;
	aref<point>			points;
	int					getframe() const;
	rect				getrect() const override { return box; }
	aref<point>			getpoints() const override { return points; }
	bool				isvisibleactive() const override { return true; }
	void				add(item value) const;
};
struct boosti {
	short unsigned		owner_id;
	variant				id;
	variant				source;
	unsigned			time;
	char				bonus;
};
struct itemcont : item {
	container*			object;
	itemcont() = default;
	itemcont(const item& value);
	void* operator new(unsigned size);
};
struct door : public selectable {
	unsigned char		cursor;
	rect				box;
	point				launch;
	point				points[2];
	aref<point>			open_points;
	aref<point>			close_points;
	aref<point>			search_open_points;
	aref<point>			search_close_points;
	aref<door_tile>		tiles;
	bool				opened;
	bool				locked;
	void				clear();
	int					getcursor() const override { return 30; }
	aref<point>			getpoints() const override { return isopen() ? open_points : close_points; }
	point				getposition() const override { return points[0]; }
	rect				getrect() const override { return box; }
	bool				isopen() const { return opened; }
	bool				isvisibleactive() const override { return true; }
	void				recount();
	void				setopened(bool state);
};
struct setting {
	enum mode_s : unsigned char {
		PanelFull, PanelActions, NoPanel
	};
	formation_s			formation;
	mode_s				panel;
	bool				show_movement;
	bool				show_path;
	bool				show_search;
	bool				zoom;
};
struct gamei : setting {
	adat<creature*, 8>	party;
	adat<creature*, 8>	selected;
	int					money;
	static void			checklocalization();
	static void			localization(const char* locale_id, bool writemode);
};
extern gamei			game;
struct targetreaction {
	variant				target;
	void				(creature::*method)(const variant& e);
	constexpr targetreaction() : target(), method(0) {}
	constexpr targetreaction(const variant& e) : target(e), method(0) {}
	void				clear();
};
struct treasure {
	int					cp, sp, gp, pp, gems, arts;
	int					mundane, minor, medium, major;
	treasure();
	void				clear();
	void				generate(int level);
	void				place();
private:
	void				place(item_s it, int count, int min, int max);
};
class nameable {
	variant				kind;
	gender_s			gender;
	const char*			name;
public:
	gender_s			getgender() const { return gender; }
	variant_s			getkind() const { return kind.type; }
	const char*			getname() const { return name; }
	race_s				getrace() const;
	int					getsubkind() const { return kind.value; }
	void				random_name();
	static const char*	random_name(gender_s gender, race_s race);
	void				setgender(gender_s v) { gender = v; }
	void				setkind(variant v) { kind = v; }
	void				setname(const char* v) { name = szdup(v); }
};
class actor : public drawable, public nameable {
	animate_s			action;
	point				position, start, dest;
	unsigned char		orientation; // What sight direction object have.
	unsigned short		frame;
	unsigned			duration; // This time stamp indicator when change frame
	int					range;
	map::node*			path;
	coloration			colors;
	targetreaction		action_target;
	//
	void				clearpath();
	bool				isready() const;
public:
	void				act(int player, const char* format, ...);
	virtual void		blockimpassable() const {}
	void				choose_apearance(const char* title, const char* step_title);
	void				clear();
	void				clearcolors();
	static res::tokens	getanimation(race_s race, gender_s gender, class_s type, int ai, int& ws);
	animate_s			getattackanimate(int number) const;
	static point		getbackward(point start, int step, int orientation);
	static point		getcamera();
	static point		getcamerasize();
	int					getcicle() const;
	int					getciclecount(int cicle) const;
	virtual class_s		getclass() const { return Fighter; }
	const coloration&	getcolors() const { return colors; }
	int					getflags() const;
	unsigned			getfps() const override { return 12; }
	static point		getforward(point start, int step, int orientation);
	int					getframe() const { return frame; }
	virtual int			gethits() const { return 0; }
	virtual int			getbodyheight() const { return 40; }
	int					getmovedistance(point destination, short unsigned minimum_reach) const;
	int					getorientation() const { return orientation; }
	virtual int			getportrait() const { return 0; }
	point				getposition() const override { return position; }
	point				getposition(int percent) const;
	static point		getposition(point start, point dst, formation_s formation, int pos);
	rect				getrect() const override;
	virtual int			getsize() const { return 1; }
	virtual int			getspeed() const { return 9; }
	const sprite*		getsprite(int& wi) const;
	sprite_type_s		getspritetype() const;
	static const sprite* getsprite(res::tokens id, int wi);
	virtual const item*	getwear(slot_s id) const { return 0; }
	virtual int			getzorder() const override;
	bool				hittest(point pt) const override;
	virtual void		interacting(const targetreaction& e) {}
	virtual bool		is(state_s id) const { return false; }
	virtual bool		isblock(point value) const { return false; }
	virtual bool		isselected() const { return false; }
	virtual bool		isstunned() const { return false; }
	bool				isvisible() const { return position.x != 0 || position.y != 0; }
	bool				iscolors() const { return colors.skin || colors.hair || colors.major || colors.minor; }
	static void			marker(int x, int y, int size, color c, bool flicking, bool double_border);
	bool				move(point destination, short unsigned maximum_range = 0, short unsigned minimum_reach = 0);
	void				painting(point screen) const override;
	void				paperdoll(int x, int y) const;
	static void			paperdoll(int x, int y, const coloration& colors, race_s race, gender_s gender, class_s type);
	static void			paperdoll(int x, int y, const coloration& colors, race_s race, gender_s gender, class_s type, animate_s animation, int orientation, item armor = NoItem, item weapon = NoItem, item offhand = NoItem, item helm = NoItem);
	void				pickcolors(const point skin, const point hair, const point major, const point minor);
	static void			pickcolors(const point skin, const point hair, const point major, const point minor, coloration& colors);
	short unsigned		random_portrait() const;
	void				render_attack(int number, point position);
	void				render_hit(bool fatal);
	void				render_path(const rect& rc, int mx, int my) const;
	void				render_marker(const rect& rc, int ox, int oy) const;
	void				say(const char* format, ...) const;
	void				set(animate_s value);
	virtual void		set(state_s value) {}
	void				set(const targetreaction& e) { action_target = e; }
	static void			setcamera(point camera);
	void				setorientation(unsigned char value) { orientation = value; }
	void				setposition(point newpos);
	static void			slide(const point camera);
	void				stop();
	void				testground();
	void				update() override;
	void				update_portrait();
	void				wait(char percent = 0);
};
struct monsteri {
	const char*					id;
	race_s						race;
	sprite_type_s				sptype;
	res::tokens					sprites[4];
	alignment_s					alignment;
	classa						classes;
	char						ability[Charisma + 1];
	std::initializer_list<item_s> equipment;
	skilla						skills;
	cflags<feat_s>				feats;
	const char*					name;
	const char*					text;
};
class creature : public actor {
	struct preparation {
		spell_s					id;
		unsigned char			count;
		unsigned char			count_maximum;
		variant					type;
		explicit operator bool() const { return count_maximum != 0; }
	};
	alignment_s					alignment;
	diety_s						diety;
	reaction_s					reaction;
	const char*					name;
	char						ability[LastAbility + 1];
	char						classes[LastClass + 1];
	char						skills[LastSkill + 1];
	unsigned					feats[LastFeat / 32 + 1];
	unsigned					state;
	short						hits, hits_rolled;
	unsigned					powers_prepared[LastSpell / 32 + 1];
	unsigned					spells_known[LastSpell / 32 + 1];
	unsigned char				spells_prepared[LastSpell + 1];
	unsigned char				sorcerers_used_powers[9];
	item						wears[LastQuickItem + 1];
	char						initiative;
	unsigned char				active_weapon;
	unsigned short				portrait;
	adat<preparation, 32>		powers;
	unsigned					experience;
	preparation*				add(spell_s id, variant type);
	void						addvar(variant id, char bonus);
	void						dispell();
	void						dress(int m);
	int							getraw(ability_s id) const;
	boosti*						find(variant id) const;
	boosti*						finds(variant id) const;
	void						recall(variant id, variant source, int modifier, unsigned rounds);
	void						finish();
public:
	explicit operator bool() const { return ability[0] != 0; }
	void						attack(creature& enemy);
	void						attack(const variant& enemy);
	void						add(item e);
	void						add(stringbuilder& sb, variant v1, variant v2, const char* title, bool sort_by_name = true) const;
	void						add(stringbuilder& sb, const aref<variant>& elements, const char* title) const;
	void						addinfo(stringbuilder& sb) const;
	void						addinfo(stringbuilder& sb, variant_s step) const;
	static void					adventure_combat();
	static void					adventure();
	bool						affect(spell_s id, int level, bool run);
	void						apply(race_s id);
	void						apply(class_s id);
	void						apply(variant type, char level, bool interactive);
	static void					blockallcreatures();
	void						blockimpassable() const override;
	void						clear();
	void						clear(variant_s value);
	void						close(const variant& e);
	variant						choose(const char* title, const char* step_title, varianta& elements) const;
	void						choose_action();
	bool						choose_ability(const char* title, const char* step_title, bool add_race);
	bool						choose_feats(const char* title, const char* step_title, varianta& elements, const unsigned* minimal, char points, bool interactive);
	static variant				choose_position(int cursor, short unsigned max_cost);
	bool						choose_skills(const char* title, const char* step_title, varianta& elements, char points, char points_per_skill, bool interactive);
	bool						choose_skills(const char* title, varianta& elements, bool interactive);
	variant						choose_target() const;
	void						create(monster_s type, reaction_s reaction);
	static creature*			create(monster_s type, reaction_s reaction, point postition);
	static void					create(monster_s type, reaction_s reaction, point postition, unsigned char orientation, int count);
	void						create(class_s type, race_s race, gender_s gender, reaction_s reaction);
	static void					create_party();
	void						damage(int count);
	void						dresson() { dress(1); }
	void						dressoff() { dress(-1); }
	bool						equip(const item e);
	void						generate(const char* title);
	int							get(ability_s id) const;
	int							get(skill_s id) const;
	int							get(class_s id) const { return classes[id]; }
	int							get(feat_s id) const { return is(id) ? 1 : 0; }
	int							get(spell_s id) const;
	item*						get(slot_s id) { return wears + id; }
	void						get(attacki& result, slot_s slot) const;
	void						get(attacki& result, slot_s slot, const creature& enemy) const;
	static ability_s			getability(ability_s id) { return bsdata<abilityi>::elements[id].base; }
	int							getac(bool flatfooted) const;
	static creature*			getactive();
	alignment_s					getalignment() const { return alignment; }
	creature*					getbest(const aref<creature*>& source, bool (creature::*proc)(const creature& opponent) const) const;
	int							getbodyheight() const;
	int							getboost(variant id) const;
	int							getcasterlevel() const;
	int							getcharlevel() const;
	class_s						getclass() const override;
	int							getcost(skill_s id) const { return isclass(id) ? 1 : 2; }
	static creature*			getcreature(point position);
	static creature*			getcreature(short unsigned index);
	diety_s						getdiety() const { return diety; }
	int							getfeats() const;
	short unsigned				getid() const;
	void						getin(const variant& e);
	int							getinitiative() const;
	int							getinitiativeroll() const { return initiative; }
	int							gethits() const override { return hits; }
	int							gethitsmax() const;
	int							getlevel() const;
	short unsigned				getindex() const;
	int							getmaxcarry() const;
	static int					getmoney();
	int							getmovement() const;
	const item&					getoffhand() const { return wears[QuickOffhand + active_weapon * 2]; }
	static int					getpartymaxdistance(point position);
	int							getpoints(class_s id) const;
	int							getportrait() const override { return portrait; }
	int							getprepared(variant type) const;
	int							getprepared(spell_s id, variant type) const;
	int							getr(ability_s id) const { return ability[id]; }
	short unsigned				getreach() const;
	int							getskillpoints() const;
	int							getspellslots(variant type, int spell_level) const;
	const sprite*				getsprite(int& wi) const;
	int							getquick() const { return active_weapon; }
	const item&					getweapon() const { return wears[QuickWeapon + active_weapon * 2]; }
	const item*					getwear(slot_s id) const override;
	bool						have(variant id) const;
	static void					help();
	void						icon(int x, int y, item* pi, slot_s id, itemdrag* pd, fnitem proc, bool show_background = true);
	void						icon(int x, int y, slot_s id, itemdrag* pd);
	void						iconqw(int x, int y, int n, itemdrag* pd);
	void						interact(const targetreaction& e, short unsigned maximum_range, bool synchronized);
	void						interacting(const targetreaction& e) override;
	bool						is(feat_s id) const { return (feats[id / 32] & (1 << (id % 32))) != 0; }
	static bool					is(spell_s id, class_s cls, int level);
	bool						is(state_s id) const override { return (state&(1 << id)) != 0; }
	bool						isallow(alignment_s id) const;
	bool						isallow(feat_s id, bool test_feats = true) const;
	bool						isallow(item_s it) const;
	bool						isallow(variant id) const;
	bool						isblock(point value) const override;
	bool						isclass(skill_s id) const;
	static bool					iscombatmode();
	bool						isenemy(const creature& opponent) const;
	bool						isranged() const { return wears[active_weapon].isranged(); }
	bool						isready(spell_s id) const;
	bool						isreadypower(spell_s id) const { return (powers_prepared[id / 32] & (1 << (id % 32))) != 0; }
	bool						isselected() const override;
	static bool					isgood(class_s id, ability_s value);
	bool						isknown(spell_s id) const { return (spells_known[id / 32] & (1 << (id % 32))) != 0; }
	bool						isplayer() const;
	bool						isready() const { return ability[Strenght] > 0 && hits > 0; }
	void						invertory();
	void						invertory(itemdrag* pd);
	static void					journal();
	static void					makecombat();
	static void					minimap();
	static void					moveto(aref<creature> players, point pt, formation_s formation);
	static void					options();
	void						random_equip(bool interactive);
	void						remove(feat_s id) { feats[id / 32] &= ~(1 << (id % 32)); }
	void						remove(state_s id) { state &= ~(1 << id); }
	bool						roll(rolli& e) const;
	void						open(const variant& e);
	void						say(const char* format, ...) const;
	static aref<creature*>		select(const aref<creature*>& destination, const aref<creature*>& source, const creature* player, bool(creature::*proc)(const creature& e) const, short unsigned range_maximum = 0, short unsigned range_index = Blocked);
	static void					select_all();
	void						set(ability_s id, int value) { ability[id] = value; }
	void						set(alignment_s value) { alignment = value; }
	void						set(class_s v, int level) { classes[v] = level; }
	void						set(feat_s id);
	void						set(state_s id) override { state |= 1 << id; }
	void						set(coloration& value) const;
	void						set(reaction_s value) { reaction = value; }
	void						set(skill_s id, int value) { skills[id] = value; }
	void						set(variant value);
	void						setactive();
	void						setactive(const variant& e) { if(e.type == Creature) e.getcreature()->setactive(); }
	void						setknown(spell_s id) { spells_known[id / 32] |= (1 << (id % 32)); }
	static void					setmoney(int value);
	void						setportrait(int value) { portrait = value; }
	void						setprepared(spell_s id, variant type, int count);
	void						setquick(int value) { active_weapon = value; }
	void						sheet();
	void						show(const char* header, item& it);
	void						spellbook();
	static void					spellinfo(spell_s id);
	void						talk(const variant& e) {}
	void						toggle(const variant& e);
	static void					moveto(const char* location, const char* entrance = 0);
	static void					updategame();
};
struct worldmap {
	struct link {
		geography_s				side;
		unsigned char			index;
		const char*				entry;
		unsigned				time;
		unsigned char			encounter_chance;
		unsigned				flags;
	};
	struct area {
		const char*				id;
		const char*				name;
		point					position;
		short unsigned			avatar;
		short unsigned			flags;
		link					links[16];
		bool					is(area_flag_s value) const { return (flags&value) != 0; }
		void					set(area_flag_s value) { flags |= value; }
	};
	const char*					background;
	const char*					name;
	const char*					icons;
	aref<area>					areas;
	static area*				getarea();
	static area*				getarea(const char* id);
	static worldmap*			getworld(area* p);
	static worldmap*			getworld() { return getworld(getarea()); }
	static worldmap*			getworld(const char* id) { return getworld(getarea(id)); }
	link*						find(const area* start, const area* dest) const;
	bool						isneighboard(const area* start, const area* dest) const { return find(start, dest) != 0; }
	static void					set(area* value);
};
struct geographyi {
	const char*					id;
	const char*					name;
};
namespace map {
void							clear();
void							drop(indext index, item it);
entrance*						find(const char* id);
const sprite*					getareasprite();
int								getday(unsigned value);
point							getentrance(const char* name, unsigned char* orient = 0);
point							getfree(point position, int size);
unsigned short					getindex(point position);
unsigned short					getindex(point position, int size);
int								gethour(unsigned value);
const sprite*					getminimap();
unsigned char					getorientation(point s, point d);
color*							getpallette();
void							getpassedtime(stringbuilder& sb, unsigned value);
point							getposition(indext index, int size);
inline int						getrange(int feets) { return (feets / 5) * 2; }
color							getshadow(point position);
unsigned char					getstate(indext index);
int								gettile(indext index);
bool							isblock(indext index, int size);
bool							load(const char* name);
unsigned char*					ptr(const char* name);
void							settile(indext index, short unsigned tile);
}
namespace draw {
inline void						background(res::tokens token, int id = 0) { image(0, 0, gres(token), id, 0); }
bool							button(int x, int y, unsigned flags, res::tokens r, int checked, int normal, int pressed, int disabled, const char* name, int key, button_s* button_state, bool pressed_execute = false);
bool							button(int x, int y, int m, unsigned flags, res::tokens r, const char* name, int key = 0, button_s* state = 0);
void							button(int x, int y, fnevent proc, int m, unsigned flags, res::tokens r, const char* name, int key = 0, button_s* state = 0);
bool							dlgask(const char* text);
int								dlgcho(const char* text, const char* a1, const char* a2);
void							dlgmsg(const char* text);
int								field(rect rc, char* result, unsigned maximum);
unsigned						getframe();
unsigned						getframe(int fps);
inline void						image(int x, int y, res::tokens token, int id, int flags = 0, unsigned char alpha = 0xFF) { draw::image(x, y, gres(token), id, flags, alpha); }
bool							inside(point t, point* points, int count);
bool							isnext(void(*proc)());
int								label(int x, int y, int width, int height, const char* name, int header = 0, int color = 0, bool border = false);
int								labell(int x, int y, int width, int height, const char* name, int header = 0, int color = 0);
int								labelr(int x, int y, int width, int height, const char* name, int header = 0, int color = 0);
void							menumodal(bool use_keys = true);
void							mslog(const char* format, ...);
void							mspaint(const rect& rc, const rect& rcs);
extern surface					pallette;
bool							picker(int x, int y, unsigned char index, int type);
void							scale2x(void* void_dst, unsigned dst_slice, const void* void_src, unsigned src_slice, unsigned width, unsigned height);
void							set(color * dest, unsigned char index, int start = 4, int count = 12);
void							textblend(point pos, const char* text, unsigned duration);
void							translate(hotkey* keys);
void							view(rect rc, rect rcs, int pixels_per_line, clist& e);
void							view(rect rc, rect rcs, const char* text, ctext& e);
}
int								compare_variant(const void* v1, const void* v2);
extern const char*				getstr(const variant& e);

inline int d100() { return rand() % 100; }

template<> void archive::set<container>(container& e);
template<> void archive::set<door>(door& e);
template<> void archive::set<entrance>(entrance& e);
template<> void archive::set<region>(region& e);
template<> void archive::set<animation>(animation& e);