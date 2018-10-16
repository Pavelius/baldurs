#include "archive.h"
#include "collection.h"
#include "converter.h"
#include "crt.h"
#include "dice.h"
#include "draw.h"
#include "io.h"
#include "map.h"
#include "resources.h"
#include "screenshoot.h"
#include "stringcreator.h"

enum school_s : unsigned char {
	NoSchool,
	SchoolAbjuration, SchoolConjuration, SchoolDivination, SchoolEnchantment,
	SchoolEvocation, SchoolIllusion, SchoolNecromancy, SchoolTransmutation,
};
enum school_type_s : unsigned char {
	Calling, Creation, HealingConjuration, Summoning,
	Charm, Compulsion,
	Figmental, Glamour, Phantasmal, Shadowing,
};
enum spell_s : unsigned short {
	NoSpell,
	// Spells level 1
	SpellArmorOfFaith, SpellBane, SpellBless, SpellBurningHands,
	SpellCharmPerson, SpellChillTouch, SpellChromaticOrb, SpellColorSpray, SpellCommand,
	SpellCureLightWounds, SpellDoom, SpellEagleSSplendor, SpellEntangle, SpellFaerieFire,
	SpellFrostFingers, SpellGenericAbjuration, SpellGrease, SpellIceDagger, SpellIdentify,
	SpellInflictLightWounds, SpellLarlochSMinorDrain, SpellMageArmor, SpellMagicMissile, SpellMagicStone,
	SpellMinorMirrorImage, SpellProtectionFromEvil, SpellProtectionFromPetrification, SpellRemoveFear, SpellSanctuary,
	SpellShield, SpellShillelagh, SpellShockingGrasp, SpellSleep, SpellSpook,
	SpellSummonMonsterI, SpellSunscorch,
	// Spells level 2
	SpellAganazzarSScorcher, SpellAid, SpellAlicornLance, SpellBarkskin, SpellBeastClaw,
	SpellBlindness, SpellBlur, SpellBullSStrength, SpellCatSGrace, SpellChant,
	SpellCharmPersonOrAnimal, SpellCureModerateWounds, SpellDeafness, SpellDeathArmor, SpellDecastave,
	SpellDelayPoison, SpellDrawUponHolyMight, SpellFindTraps, SpellFlameBlade, SpellGedleeSElectricLoop,
	SpellGhoulTouch, SpellGoodberry, SpellHoldPerson, SpellHorror, SpellInflictModerateWounds,
	SpellInvisibility, SpellKnock, SpellLuck, SpellMelfSAcidArrow, SpellMinorElementalBarrier,
	SpellMirrorImage, SpellMoonMotes, SpellPowerWordSleep, SpellRainstorm, SpellRayOfEnfeeblement,
	SpellRemoveFatigue, SpellResistFear, SpellSeeInvisibility, SpellSilence, SpellSnillocSSnowballSwarm,
	SpellSpellShield, SpellSpiritualWeapon, SpellStinkingCloud, SpellSummonCow, SpellSummonMonsterII,
	SpellTortoiseShell, SpellVocalize, SpellWeb,
	// Spells level 3
	SpellAnimateDead, SpellBlink, SpellCallLightning, SpellCircleOfBones, SpellContagion,
	SpellDireCharm, SpellDispelMagic, SpellExaltation, SpellExplosiveRunes, SpellFireball,
	SpellFlameArrow, SpellGhostArmor, SpellGlyphOfWarding, SpellHaltUndead, SpellHaste,
	SpellHoldAnimal, SpellHolySmite, SpellIcelance, SpellInvisibilityPurge, SpellInvisibilitySphere,
	SpellLanceOfDisruption, SpellLightningBolt, SpellMelfSMinuteMeteors, SpellMiscastMagic, SpellMoldTouch,
	SpellMoonblade, SpellNegativeEnergyProtection, SpellNondetection, SpellPrayer, SpellProtectionFromArrows,
	SpellProtectionFromFire, SpellRemoveCurse, SpellRemoveDisease, SpellRemoveParalysis, SpellRigidThinking,
	SpellSkullTrap, SpellSlow, SpellSpikeGrowth, SpellStormShell, SpellStrengthOfOne,
	SpellSummonMonsterIII, SpellUnholyBlight, SpellVampiricTouch,
	// Spells level 4
	SpellBeltynSBurningBlood, SpellBloodRage, SpellCloakOfFear, SpellCloudOfPestilence, SpellCureSeriousWounds,
	SpellDeathWard, SpellDefensiveHarmony, SpellDimensionDoor, SpellEmotionDespair, SpellEmotionFear,
	SpellEmotionHope, SpellEmotionRage, SpellFireShieldBlue, SpellFireShieldRed, SpellFreedomOfMovement,
	SpellGiantVermin, SpellHolyPower, SpellIceBlade, SpellIceStorm, SpellImprovedInvisibility,
	SpellInflictSeriousWounds, SpellMagicCircleAgainstEvil, SpellMalison, SpellMentalDomination, SpellMinorGlobeOfInvulnerability,
	SpellMordenkainenSForceMissiles, SpellNeutralizePoison, SpellOtilukeSResilientSphere, SpellPoison, SpellProduceFire,
	SpellProtectionFromLightning, SpellRecitation, SpellRestoration, SpellShadowConjuration, SpellShout,
	SpellSpiderSpawn, SpellSpiritArmor, SpellStarMetalCudgel, SpellStoneskin, SpellSummonMonsterIV,
	SpellSummonNatureSAllyI, SpellSummonNatureSAllyII, SpellSummonNatureSAllyIII, SpellSummonNatureSAllyIV, SpellThornSpray,
	SpellVitriolicSphere, SpellWallOfFire, SpellWallOfMoonlight,
	FirstSpell = SpellArmorOfFaith, LastSpell = SpellWallOfMoonlight,
};
enum ability_s : unsigned char {
	Strenght, Dexterity, Constitution, Intellegence, Wisdow, Charisma,
};
enum class_s : unsigned char {
	// Classes
	Commoner,
	Adept, Aristocrat, Beast, Monster, Undead, Warrior,
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
enum feat_s : unsigned char {
	NoFeat,
	// Feats
	Alertness,
	ArmorProfeciencyLight, ArmorProfeciencyMedium, ArmorProfeciencyHeavy,
	BlindFight,
	Cleave, CombatCasting, CombatExpertise, CombatReflexes,
	DeadlyPercision, DeflectsArrows, Dodge, Endurance,
	FarShoot,
	GreateFortitude,
	ImprovedCritical, ImprovedDisarm, ImprovedFeint, InprovedInitiative, ImprovedOverrun, ImprovedSunder, ImprovedTrip, ImprovedUnarmedStrike,
	IronWill,
	Leadership, LightingReflexes,
	Mobiliy, Multiattack,
	PointBlankShoot, PowerAttack, PreciseShoot,
	ProficiencyAxe, ProficiencyClub, ProficiencyCrossbow,
	ProficiencyDagger, ProficiencyGreatweapon, ProficiencyHeavyCrossbow,
	ProficiencyMace, ProficiencySimple, ProficiencySpear,
	ProficiencyLongbow, ProficiencyLongsword,
	ProficiencyScimitar, ProficiencyShortbow, ProficiencyShortsword,
	ProficiencyBastardsword, ProficiencyCatana, ProficiencyWaraxe,
	RapidShoot, RapidReload,
	ShieldProfeciency,
	ShortOnTheRun,
	SpellFocus,
	SpringAttack, StunningFist,
	Toughness, Track, TwoWeaponFighting,
	FocusAxes, FocusBows, FocusCrossbows, FocusDaggers, FocusGreatswords,
	FocusLongswords, FocusMaces, FocusPolearm, FocusShortswords,
	WhirlwindAttack,
	FirstFeat = Alertness, LastFeat = WhirlwindAttack,
};
enum race_s : unsigned char {
	NoRace,
	Dwarf, Elf, Gnome, HalfElf, HalfOrc, Halfling, Human,
	FirstRace = Dwarf, LastRace = Human,
};
enum gender_s : unsigned char {
	NoGender,
	Male, Female,
};
enum alignment_s : unsigned char {
	NoAlignment,
	LawfulGood, NeutralGood, ChaoticGood, LawfulNeutral, TrueNeutral, ChaoticNeutral, LawfulEvil, NeutralEvil, ChaoticEvil,
};
enum god_s : unsigned char {
	NoGod,
	GodBane, GodChauntea, GodIllmater, GodGruumsh, GodHelm,
	GodKelemvor, GodKossuth, GodLathander, GodLolth, GodMask,
	GodMystra, GodMoradin, GodOghma, GodShar, GodTempus,
	GodTymora, GodTyr, GodUmberly,
};
enum item_s : unsigned char {
	NoItem,
	Club, Hammer, Mace, Spear, Staff, CrossbowLight, CrossbowHeavy, Sling,
	BattleAxe, Dagger, Greataxe, Halberd,
	Scimitar, Longsword, Shortsword, Greatsword, Rapier, Shortbow, Longbow,
	Waraxe, BastardSword, Katana,
	// Items (armor)
	LeatherArmor, StuddedLeatherArmor, ChainShirt,
	ScaleMail, ChainMail, Breastplate,
	SplintMail, BandedMail, Plate,
	ShieldSmall, ShieldLarge, ShieldTower,
	Helm,
	LastItem = Helm,
};
enum animation_s : unsigned char {
	AnimateMove,
	AnimateStand, AnimateStandRelax, AnimateStandLook,
	AnimateCombatStance, AnimateCombatStanceTwoHanded,
	AnimateGetHit, AnimateGetHitAndDrop,
	AnimateAgony,
	AnimateGetUp,
	AnimateMeleeOneHanded, AnimateMeleeOneHandedSwing, AnimateMeleeOneHandedThrust,
	AnimateMeleeTwoHanded, AnimateMeleeTwoHandedSwing, AnimateMeleeTwoHandedThrust,
	AnimateMeleeTwoWeapon, AnimateMeleeTwoWeaponSwing, AnimateMeleeTwoWeaponThrust,
	AnimateShootBow, AnimateShootSling, AnimateShootXBow,
	AnimateCastBig, AnimateCastBigRelease, AnimateCast, AnimateCastRelease, AnimateCastThird, AnimateCastThirdRelease, AnimateCastFour, AnimateCastFourRelease
};
enum slot_s : unsigned char {
	Backpack, LastBackpack = Backpack + 23,
	Head, Neck, Body, Rear, LeftFinger, RightFinger, Hands, Gridle, Legs,
	QuickWeapon, QuickOffhand, LastQuickWeapon = QuickWeapon + 4 * 2,
	Quiver, LastQuiver = Quiver + 2,
	QuickItem, LastQuickItem = QuickItem + 2,
};
enum magic_s : unsigned char {
	Mundane, Cursed, Magical, Artifact,
};
enum state_s : unsigned char {
	Stunned,
};
enum item_group_s : unsigned char {
	MiscItem,
	SimpleWeapon, MartialWeapon, ExoticWeapon,
};
enum action_s : unsigned char {
	ActionGuard, ActionTurnUndead, ActionCast, ActionAttack, ActionUseItem, ActionPerform, ActionStop,
};
enum formation_s : unsigned char {
	FormationFollow, FormationT, FormationGather, Formation4and2, Formation3by2, FormationProtect,
};
enum color_indexes {
	ColorRedTintedBlack, ColorDarkBronze, ColorDarkGold, ColorLightGold, ColorAuburn,
	ColorLightSilver, ColorDarkSilver, ColorLightMetallicGreen, ColorDarkMuddishBrown, ColorLightMuddishBrown,
	ColorLightPureGold, ColorLightRoseRed, ColorLightCarnationPink, ColorLightPureSilver, ColorDarkPureSilver,
	ColorEasterGreen, ColorSilverGold, ColorLightBlue, ColorDarkBlue, ColorDarkRoseRed,
	ColorDarkMoldyGreen, ColorDarkIronGrey, ColorDarkBrown, ColorLightCooper, ColorDarkGold2,
	ColorDarkPureGold, ColorWood, ColorSilver, ColorGhostlyGreishSilver, ColorRedTintedBlack2,
	ColorLightIronGrey, ColorLightSeaBlue, ColorDarkSeaGreen, ColorDarkMettallicPurple,
	ColorGhostlyGreen, ColorDarkGhostlyPink, ColorLightDirtyYellow, ColorDarkDirtyYellow, ColorLightDirtishBrown,
};
namespace res {
enum tokens {
	NONE,
	CURSORS, CURSARW, COLGRAD, COLOR,
	GACTN,
	GBTNBFRM, GBTNLRG, GBTNMED, GBTNMED2, GBTNMINS, GBTNPLUS, GBTNOPT1, GBTNOPT2,
	GBTNSPB1, GBTNSPB2, GBTNSPB3,
	GBTNPOR, GUIMAP, GUIMAPWC, GUIREC, GBTNRECB, GBTNSCRL, GBTNSTD,
	GCOMM, GCOMMBTN,
	INVBUT2, INVBUT3, ITEMS, SCROLLS, FORM,
	CDMB1, CDMB2, CDMB3, CDMC4, CDMF4, CDMT1, CDMW1, CDMW2, CDMW3, CDMW4,
	CEFB1, CEFB2, CEFB3, CEFC4, CEFF4, CEFT1, CEFW1, CEFW2, CEFW3, CEFW4,
	CEMB1, CEMB2, CEMB3, CEMC4, CEMF4, CEMT1, CEMW1, CEMW2, CEMW3, CEMW4,
	CHFB1, CHFB2, CHFB3, CHFC4, CHFF4, CHFT1, CHFW1, CHFW2, CHFW3, CHFW4, CHFM1,
	CHMB1, CHMB2, CHMB3, CHMC4, CHMF4, CHMT1, CHMW1, CHMW2, CHMW3, CHMW4, CHMM1,
	CIFB1, CIFB2, CIFB3, CIFC4, CIFF4, CIFT1,
	CIMB1, CIMB2, CIMB3, CIMC4, CIMF4, CIMT1,
	GUIA, GUIBTACT, GUIBTBUT,
	GUICGB, GUICHP, GUIHELP, GUIFEAT, GUIINV, GUIHSB, GUICARBB,
	GUIPFC, GUIRSPOR, GUISEX, GUISPL, GUICNAME, GUIERR,
	GUIRLVL, GMPMCHRB, GCGPARTY, GBTNLRG2,
	GUIJRLN, GOPT, GUIMOVB, STONEOPT, GUISRSVB, GUISRRQB,
	CGEAR,
	LOAD, LOCATER,
	PORTL, PORTS,
	SPELLS, SPLBUT, START, STON, STONSLOT, WMAP,
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
	Count
};
}
enum region_type_s {
	RegionTriger, RegionInfo, RegionTravel
};
enum button_states {
	ButtonChecked, ButtonNormal, ButtonPressed, ButtonDisabled
};
enum save_s : unsigned char {
	Fortitude, Reflexes, Will,
};
enum tile_s : unsigned char {
	ObstacleImpassableBlack, Sand, WoodGreen, WoodBrown,
	StoneDry, Grass, WaterTurquose, Stone,
	ObstacleImpassableGrey, WoorRed, Wall, WaterYellow,
	WaterImpassable, RoofImpassable, WorldExit, GrassHardMove,
};
enum variant_s : unsigned char {
	NoVariant,
	Ability, Alignment, Apearance, Class, Gender, Feat, Item, Race, Skill, Spell, Name,
};
typedef aset<char, LastSkill + 1, skill_s> skillset;
struct variant {
	variant_s			type;
	union {
		ability_s		ability;
		alignment_s		alignment;
		class_s			clas;
		feat_s			feat;
		gender_s		gender;
		race_s			race;
		skill_s			skill;
		spell_s			spell;
		item_s			item;
		unsigned char	number;
	};
	constexpr variant() : type(NoVariant), number(0) {}
	constexpr variant(unsigned short integer) : type((variant_s)(integer >> 8)), number(integer & 0xFF) {}
	constexpr variant(variant_s t, unsigned char v) : type(t), number(v) {}
	constexpr variant(ability_s value) : type(Ability), ability(value) {}
	constexpr variant(alignment_s value) : type(Alignment), alignment(value) {}
	constexpr variant(class_s value) : type(Class), clas(value) {}
	constexpr variant(gender_s value) : type(Gender), gender(value) {}
	constexpr variant(feat_s value) : type(Feat), feat(value) {}
	constexpr variant(race_s value) : type(Race), race(value) {}
	constexpr variant(skill_s value) : type(Skill), skill(value) {}
	constexpr variant(spell_s value) : type(Spell), spell(value) {}
	constexpr variant(item_s value) : type(Item), item(value) {}
	bool operator==(const variant& e) const { return type == e.type && number == e.number; }
	operator unsigned short() { return (type << 8) + number; }
};
struct runable {
	virtual void		execute() const = 0;
	virtual int			getid() const { return 0; }
};
struct cmpr : runable {
	constexpr cmpr(void(*proc)(), int param = 0) : proc(proc), param(param) {}
	void				execute() const override { draw::execute(proc, param); }
	int					getid() const override { return param; }
private:
	void(*proc)();
	int					param;
};
struct drawable {
	virtual int			getcursor() const { return 0; } // Get cursor index when over this drawable
	virtual const char*	getdescription() const { return ""; }
	virtual const char*	getid() const { return ""; }
	virtual unsigned	getfps() const { return 20; }
	virtual point		getposition() const = 0;
	virtual rect		getrect() const = 0;
	virtual int			getzorder() const { return 100; }// Priority for z-order sortering (lesser was be most visible). If there is two drawable in same position.
	virtual bool		hittest(point position) const { return false; }
	virtual bool		isvisible() const { return false; }
	virtual bool		isvisibleactive() const { return false; } // Drawable visible only when active.
	virtual void		painting(point position) const = 0; // How to paint drawable.
	virtual void		update() {}
};
struct scrolltext {
	int					origin;
	int					maximum;
	int					increment;
	res::tokens			bar;
	const char*			cache_text;
	int					cache_height;
	int					cache_origin;
	int					scroll_frame;
	//
	scrolltext();
	void				reset();
};
struct scrolllist : runable, scrolltext {
	virtual void		row(rect rc, int n) = 0;
	virtual void		execute() const override {}
	virtual int			getid() const override { return 0; }
};
struct scrollbutton : runable, scrolltext {
	virtual void		row(rect rc, int n) = 0;
};
struct cursorset {
	cursorset(res::tokens r = res::CURSORS, int f = 267);
	~cursorset();
	void				set(res::tokens r = res::CURSORS, int f = 267);
private:
	res::tokens			r;
	int					f;
};
struct coloration {
	unsigned char		skin;
	unsigned char		hair;
	unsigned char		minor;
	unsigned char		major;
	unsigned char		metal;
	unsigned char		leather;
	unsigned char		armor;
	//
	explicit operator bool() const { return skin == hair == minor == major == 0; }
	constexpr coloration() : skin(0), hair(0), minor(0), major(0), armor(28), metal(30), leather(23) {}
	void				set(unsigned char skin, unsigned char hair, unsigned char minor, unsigned char major);
	void				upload(color* col) const;
};
struct door_tile {
	short unsigned		index; // index = y*64 + x;
	short unsigned		open; // new tile index
	short unsigned		closed; // new tile index
};
struct hotkey {
	void(*proc)();
	unsigned			key;
	const char*			name;
	explicit operator bool() const { return proc != 0; }
};
struct floattext : drawable {
	const char*			text;
	unsigned			stamp;
	rect				box;
	constexpr floattext() : box(), text(0), stamp(0) {}
	explicit operator bool() const { return text != 0; }
	void				clear();
	bool				isvisible() const override { return stamp && text; }
	point				getposition() const override { return{(short)(box.x1 + box.width() / 2), (short)(box.y2 + 128)}; }
	rect				getrect() const override { return{box.x1 - 4, box.y1 - 4, box.x2 + 4, box.y2 + 4}; }
	void				painting(point offset) const override;
	void				update() override;
};
struct animation : drawable, point {
	struct info {
		const sprite*	source;
		int				frame;
		unsigned		flags;
		unsigned char	transparent;
		explicit operator bool() const { return source != 0; }
	};
	unsigned			schedule;
	char				rsname[9], rsname_pallette[9];
	unsigned short		circle, frame, start_frame;
	unsigned			flags;
	unsigned short		height;
	unsigned char		transparency;
	unsigned char		chance_loop;
	unsigned char		skip_cycles;
	constexpr animation() : point(), schedule(0), circle(0), frame(0), start_frame(0),
		flags(0), height(0), transparency(0), chance_loop(0), skip_cycles(0),
		rsname(), rsname_pallette() {}
	void				getinfo(info& e) const;
	rect				getrect() const override;
	point				getposition() const override { return {x, y + height}; }
	bool				is(unsigned value) const { return (flags & value) != 0; }
	virtual bool		isvisible() const override;
	void				painting(point screen) const override;
};
struct selectable : public drawable {
	virtual aref<point>	getpoints() const = 0;
	point				getposition() const override;
	bool				hittest(point hittest) const;
	void				painting(point screen) const override;
};
struct item {
	explicit operator bool() const { return type != NoItem; }
	constexpr item() : type(NoItem), effect(0), count(0), identified(0), magic(Mundane), quality(0) {}
	constexpr item(item_s i) : type(i), effect(0), count(0), identified(0), magic(Mundane), quality(0) {}
	void				clear();
	int					getac() const;
	int					getarmorindex() const;
	int					getbonus() const;
	int					getcount() const { return count; }
	feat_s				getfeat() const;
	static const char*	getfname(int type);
	int					getframe() const;
	magic_s				getmagic() const { return magic; }
	int					getportrait() const { return type * 2; }
	item_s				gettype() const { return type; }
	bool				is(feat_s value) const;
	bool				is(slot_s value) const;
private:
	item_s				type;
	unsigned char		effect;
	unsigned char		count;
	struct {
		unsigned char	identified : 1;
		magic_s			magic : 2;
		unsigned char	quality : 2;
	};
};
struct spell_info {
	const char*			id;
	const char*			name;
	school_s			school;
	int					frame;
	aset<char, LastClass+1, class_s> levels;
};
struct portrait_info {
	const char*			name;
	unsigned char		skin;
	unsigned char		hair;
	unsigned char		major;
	unsigned char		minor;
	gender_s			gender;
	race_s				races[4];
	class_s				perks[8];
	bool				is(gender_s id) const;
	bool				is(race_s id) const;
	static aref<portrait_info> getelements();
};
struct race_info {
	const char*			id;
	const char*			name;
	char				abilities[6];
	skillset			skills;
	feat_s				feats[8];
	const char*			text;
};
struct class_info {
	struct slot_info {
		char		minimal;
		aref<char>	progress;
	};
	const char*			id;
	const char*			name;
	char				hd;
	char				skill_points;
	slot_info*			spells;
	ability_s			spell_ability;
	aref<skill_s>		class_skills;
	aref<feat_s>		weapon_proficiency;
	aref<feat_s>		armor_proficiency;
};
struct skill_info {
	const char*			id;
	const char*			name;
	ability_s			ability;
	skill_s				synergy[3];
};
struct entrance {
	char				name[32];
	unsigned char		orientation;
	point				position;
};
struct region : selectable {
	region_type_s		type;
	const char*			name;
	point				launch;
	rect				box;
	char				move_to_area[8];
	char				move_to_entrance[32];
	aref<point>			points;
	const char*			getid() const override { return name; }
	rect				getrect() const override { return box; }
	int					getcursor() const override;
	aref<point>			getpoints() const override { return points; }

};
struct container : selectable {
	int					type;
	char				name[32];
	point				launch;
	rect				box;
	aref<point>			points;
	rect				getrect() const override { return box; }
	aref<point>			getpoints() const override { return points; }
	bool				isvisibleactive() const override { return true; }
};
struct door : public selectable {
	unsigned char		cursor;
	rect				box;
	point				launch;
	point				points[2];
	aref<point>			open_points;
	aref<point>			close_points;
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
	void				toggle();
};
struct setting {
	enum mode_s : unsigned char {
		PanelFull, PanelActions, NoPanel
	};
	mode_s				panel;
	bool				show_search;
};
struct actor : drawable {
	//
	void				act(int player, const char* format, ...);
	void				choose_apearance(const char* title, const char* step_title);
	void				clearpath();
	void				clearcolors();
	static res::tokens	getanimation(item_s type);
	static res::tokens	getanimation(race_s race, gender_s gender, class_s type, int ai, int& ws);
	static point		getcamera();
	static point		getcamerasize();
	int					getcicle() const;
	int					getciclecount(int cicle) const;
	virtual class_s		getclass() const { return Fighter; }
	int					getflags() const;
	unsigned			getfps() const override { return 12; }
	int					getframe() const { return frame; }
	virtual gender_s	getgender() const { return Male; }
	virtual int			gethits() const { return 0; }
	virtual const char* getname() const { return ""; }
	virtual int			getportrait() const { return 0; }
	point				getposition() const override;
	static point		getposition(point start, point dst, int formation, int pos);
	static point		getposition(point start, int step, int orientation);
	virtual race_s		getrace() const { return Human; }
	rect				getrect() const override;
	virtual int			getsize() const { return 2; }
	virtual int			getspeed() const { return 10; }
	static const sprite* getsprite(res::tokens id, int wi);
	virtual const item	getwear(slot_s id) const { return NoItem; }
	bool				hittest(point pt) const override;
	bool				isblock(point value) const;
	virtual bool		isstunned() const { return false; }
	bool				isvisible() const { return true; }
	bool				iscolors() const { return colors.skin || colors.hair || colors.major || colors.minor; }
	void				move(point destination);
	void				painting(point screen) const override;
	void				paperdoll(int x, int y, animation_s animation = AnimateStand, int orientation = 2) const;
	static void			paperdoll(int x, int y, const coloration& colors, race_s race, gender_s gender, class_s type, animation_s animation = AnimateStand, int orientation = 2, item armor = NoItem, item weapon = NoItem, item offhand = NoItem, item helm = NoItem);
	void				pickcolors(const point skin, const point hair, const point major, const point minor);
	static void			pickcolors(const point skin, const point hair, const point major, const point minor, coloration& colors);
	void				render_marker(const rect& rc, int ox, int oy) const;
	void				say(const char* format, ...) const;
	void				set(animation_s value);
	virtual void		set(gender_s value) {}
	void				set(point value);
	void				set(state_s value, unsigned rounds) {}
	static void			setcamera(point camera);
	void				update() override;
	void				update_action();
	void				update_animation();
	void				update_portrait();
private:
	animation_s			action;
	point				position, start, dest;
	unsigned char		orientation; // What sight direction object have.
	unsigned short		frame;
	unsigned			duration; // This time stamp indicator when change frame
	int					range;
	const sprite*		sprites[4];
	map::node*			path;
	coloration			colors;
	//
	void				paint_circle(int x, int y) const;
};
struct creature : actor {
	explicit operator bool() const { return ability[0] != 0; }
	void* operator new(unsigned size);
	void operator delete (void* data);
	void				add(item e);
	void				add(stringbuilder& sb, variant v1) const;
	void				add(stringbuilder& sb, variant v1, variant v2, const char* title) const;
	void				add(stringbuilder& sb, const aref<variant>& elements, const char* title) const;
	void				addinfo(stringbuilder& sb) const;
	static void			adventure();
	void				apply(race_s id, bool add_ability);
	void				apply(class_s id);
	void				apply(variant type, char level, bool interactive);
	void				clear();
	void				clear(variant_s value);
	bool				choose_feats(const char* title, const char* step_title, const unsigned* minimal, char points);
	bool				choose_skills(const char* title, const char* step_title, const char* minimal, char points, char points_per_skill);
	void				choose_skills(const char* title);
	void				create(class_s type, race_s race, gender_s gender);
	static void			create_party();
	bool				equip(const item e);
	void				generate(const char* title);
	int					get(ability_s id) const;
	int					get(save_s id) const;
	int					get(skill_s id) const;
	int					get(class_s id) const { return classes[id]; }
	int					get(feat_s id) const { return is(id) ? 1 : 0; }
	int					get(spell_s id) const;
	static ability_s	getability(save_s id);
	int					getac(bool flatfooted);
	static creature*	getplayer();
	int					getbab() const;
	int					getcasterlevel() const;
	int					getcharlevel() const;
	class_s				getclass() const override;
	int					getcost(skill_s id) const { return isclass(id) ? 1 : 2; }
	void				getdescription(stringbuilder& sb) const;
	int					getfeats() const;
	gender_s			getgender() const override { return gender; }
	god_s				getgod() const { return god; }
	int					gethits() const override { return hits; }
	int					gethitsmax() const;
	int					getlevel() const;
	const char*			getname() const override { return name; }
	int					getmaxcarry() const;
	const item&			getoffhand() const { return wears[QuickOffhand + active_weapon * 2]; }
	int					getpoints(class_s id) const;
	int					getportrait() const override { return portrait; }
	int					getprepared(variant type) const;
	int					getprepared(spell_s id, variant type) const;
	int					getr(ability_s id) const { return ability[id]; }
	race_s				getrace() const override { return race; }
	int					getskillpoints() const;
	int					getspellslots(variant type, int spell_level) const;
	variant_s			getstep() const;
	int					getquick() const { return active_weapon; }
	const item&			getweapon() const { return wears[QuickWeapon + active_weapon * 2]; }
	static void			help();
	void				icon(int x, int y, slot_s id);
	void				icon(int x, int y, const runable& cmd, slot_s id) { icon(x, y, cmd, wears + id, id); }
	void				icon(int x, int y, const runable& cmd, item* pi, slot_s id);
	void				iconqw(int x, int y, int n);
	bool				is(feat_s id) const { return (feats[id / 32] & (1 << (id % 32))) != 0; }
	static bool			is(spell_s id, class_s cls, int level);
	bool				isallow(feat_s id) const;
	static bool			isallow(feat_s id, const unsigned char* ability, char character_level, char base_attack);
	bool				isallow(const item& it) const;
	bool				isallow(variant id) const;
	bool				isclass(skill_s id) const;
	static bool			isgood(class_s id, save_s value);
	bool				isknown(spell_s id) const { return (spells_known[id / 32] & (1 << (id % 32)))!=0; }
	void				invertory();
	static void			minimap();
	static void			options();
	void				remove(feat_s id) { feats[id / 32] &= ~(1 << (id % 32)); }
	void				say(const char* format, ...) const;
	void				set(ability_s id, int value) { ability[id] = value; }
	void				set(alignment_s value) { alignment = value; }
	void				set(feat_s id) { feats[id / 32] |= (1 << (id % 32)); }
	void				set(gender_s value) override { gender = value; }
	void				set(coloration& value) const;
	void				set(race_s value) { race = value; }
	void				set(skill_s id, int value) { skills[id] = value; }
	void				set(variant value);
	void				setknown(spell_s id) { spells_known[id / 32] |= (1 << (id % 32)); }
	void				setportrait(int value) { portrait = value; }
	void				setprepared(spell_s id, variant type, int count);
	void				setquick(int value) { active_weapon = value; }
	void				sheet();
	void				spellbook();
	static void			spellinfo(spell_s id);
	static void			moveto(const char* location, const char* entrance = 0);
	static void			updategame();
private:
	struct preparation {
		spell_s			id;
		variant			type;
		unsigned char	count;
		unsigned char	count_maximum;
		explicit operator bool() const { return count_maximum != 0; }
	};
	gender_s			gender;
	race_s				race;
	alignment_s			alignment;
	god_s				god;
	const char*			name;
	unsigned char		ability[Charisma + 1];
	char				classes[LastClass + 1];
	char				skills[LastSkill + 1];
	unsigned			feats[LastFeat / 32 + 1];
	short				hits, hits_rolled;
	unsigned			spells_known[LastSpell / 32 + 1];
	item				wears[LastQuickItem + 1];
	unsigned char		active_weapon;
	unsigned char		portrait;
	unsigned char		sorcerers_used_powers[9];
	adat<preparation, 24> powers;
	unsigned			experience;
	//
	preparation*		add(spell_s id, variant type);
	void				random_name();
	static const char*	random_name(gender_s gender, race_s race);
	void				update_levels();
};
namespace map {
void					clear();
entrance*				find(const char* id);
const sprite*			getareasprite();
point					getentrance(const char* name, unsigned char* orient = 0);
point					getfree(point position, int size);
unsigned short			getindex(point position);
unsigned short			getindex(point position, int size);
const sprite*			getminimap();
unsigned char			getorientation(point s, point d);
color*					getpallette();
point					getposition(short unsigned index, int size);
color					getshadow(point position);
unsigned char			getstate(short unsigned index);
int						gettile(short unsigned index);
bool					isblock(unsigned short index, int size);
bool					load(const char* name);
unsigned char*			ptr(const char* name);
void					settile(short unsigned index, short unsigned tile);
}
namespace draw {
inline void				background(res::tokens token, int id = 0) { image(0, 0, gres(token), id, 0); }
int						button(int x, int y, const runable& cmd, unsigned flags, res::tokens r, const char* name, int key = 0, button_states* state = 0);
int						button(int x, int y, const runable& cmd, unsigned flags, res::tokens r, int checked, int normal, int pressed, int disabled, const char* name, int key, button_states* button_state, bool pressed_execute = false);
bool					dlgask(const char* text);
int						dlgcho(const char* text, const char* a1, unsigned f1, const char* a2, unsigned f2);
void					dlgmsg(const char* text);
void					ellipse(const rect& rc, color c);
int						field(rect rc, char* result, unsigned maximum);
unsigned				getframe();
unsigned				getframe(int fps);
inline void				image(int x, int y, res::tokens token, int id, int flags = 0, unsigned char alpha = 0xFF) { draw::image(x, y, gres(token), id, flags, alpha); }
bool					inside(point t, point* points, int count);
bool					isnext(void(*proc)());
int						label(int x, int y, int width, int height, const char* name, int header = 0, int color = 0, bool border = false);
int						labell(int x, int y, int width, int height, const char* name, int header = 0, int color = 0);
int						labelr(int x, int y, int width, int height, const char* name, int header = 0, int color = 0);
void					menumodal();
void					mslog(const char* format, ...);
void					mslogv(const char* temp);
void					mspaint(const rect& rc, const rect& rcs);
extern surface			pallette;
void					picker(int x, int y, unsigned char index, int type, const runable& cmd);
void					set(color * dest, unsigned char index, int start = 4, int count = 12);
void					setnext(void(*proc)());
void					textblend(point pos, const char* text, unsigned duration);
void					translate(hotkey* keys);
void					view(rect rc, scrollbutton& e);
void					view(rect rc, rect rcs, int pixels_per_line, scrolllist& e);
void					view(rect rc, rect rcs, const char* text, scrolltext& e);
}
namespace game {
int						getday(unsigned value);
int						gethour(unsigned value);
char*					getname(char* result, int rec);
const char*				getnamepart(int rec);
bool					moveto(const char* area, const char* entrance = 0);
}
int							compare_variant(const void* v1, const void* v2);
template<class T> void		getrule(stringbuilder& sb, T value);
extern adat<animation, 128>	animation_data;
extern class_info			class_data[];
extern adat<creature, 256>	creature_data;
extern adat<container, 128>	container_data;
extern adat<door, 256>		door_data;
extern adat<door_tile, 1024> door_tiles_data;
extern adat<entrance, 128>	entrance_data;
extern adat<floattext, 32>	floattext_data;
extern creature				players[6];
extern portrait_info		portrait_data[];
extern race_info			race_data[];
extern adat<region, 256>	region_data;
extern setting				settings;
extern skill_info			skill_data[];
extern spell_info			spell_data[];
extern adat<point, 256 * 256>	verticles;

template<> void archive::set<container>(container& e);
template<> void archive::set<door>(door& e);
template<> void archive::set<entrance>(entrance& e);
template<> void archive::set<region>(region& e);
template<> void archive::set<animation>(animation& e);