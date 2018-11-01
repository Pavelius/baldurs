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

#ifdef _DEBUG
#define msdbg(text, ...) draw::mslog(text, __VA_ARGS__)
#else
#define msdbg(text, ...);
#endif // _DEBUG

const int CP = 1;
const int SP = 10;
const int GP = 100;
const int PP = 1000;

enum school_s : unsigned char {
	NoSchool,
	Abjuration, Conjuration, Divination, Enchantment,
	Evocation, Illusion, Necromancy, Transmutation,
};
enum school_type_s : unsigned char {
	Calling, Creation, HealingConjuration, Summoning,
	Charm, Compulsion,
	Figmental, Glamour, Phantasmal, Shadowing,
};
enum spell_s : unsigned short {
	NoSpell,
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
	FocusAxes, FocusBows, FocusCrossbows, FocusDaggers, FocusGreatswords,
	FocusLongswords, FocusMaces, FocusPolearm, FocusShortswords,
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
enum diety_s : unsigned char {
	NoGod,
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
enum magic_s : unsigned char {
	Mundane, Cursed, Magical, Artifact,
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
	Character,
	Goblin, LargeSkeleton,
	FirstMonster = Goblin, LastMonster = LargeSkeleton,
};
enum special_s : unsigned char {
	NoSpecial,
	OfBaneOrc,
	OfFlaming, OfFlamingBurst, OfFrost, OfIcyBurst, OfShock, OfGhostTouch,
	OfHoly, OfKeen, OfShockingBurst, OfSpeed, OfThundering, OfUnholy, OfWounding, OfVorpal,
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
	GUICGB, GUICHP, GUIHELP, GUIFEAT, GUIINV, GUIHSB, GUICARBB,
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
	// Монстры
	MGO1, MSKA, MSKAA,
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
enum reaction_s : unsigned char {
	Undifferent, Friendly, Helpful, Unfriendly, Hostile,
};
enum tile_s : unsigned char {
	ObstacleImpassableBlack, Sand, WoodGreen, WoodBrown,
	StoneDry, Grass, WaterTurquose, Stone,
	ObstacleImpassableGrey, WoorRed, Wall, WaterYellow,
	WaterImpassable, RoofImpassable, WorldExit, GrassHardMove,
};
enum variant_s : unsigned char {
	NoVariant,
	Ability, Alignment, Apearance, Class, Gender, Feat, Item, Race, Skill, Spell, Name, Finish, Variant,
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
		variant_s		var;
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
	constexpr variant(variant_s value) : type(Variant), var(value) {}
	bool operator==(const variant& e) const { return type == e.type && number == e.number; }
	operator unsigned short() { return (type << 8) + number; }
};
enum target_s : unsigned char {
	NoTarget, Creature, Container, Door, ItemCont, ItemGround, Position, Region,
};
struct target {
	target_s			type;
	union {
		struct creature*	creature;
		struct point		position;
		struct container*	container;
		struct door*		door;
		struct region*		region;
		struct itemground*	itemground;
		struct itemcont*	itemcont;
	};
	target(struct drawable* value);
	constexpr target() : type(NoTarget), creature(0) {}
	constexpr target(struct creature* value) : type(Creature), creature(value) {}
	constexpr target(const point& value) : type(Position), position(value) {}
	constexpr target(struct door* value) : type(Door), door(value) {}
	constexpr target(struct region* value) : type(Region), region(value) {}
	constexpr target(struct container* value) : type(Container), container(value) {}
	constexpr target(struct itemground* value) : type(ItemGround), itemground(value) {}
	constexpr target(struct itemcont* value) : type(ItemCont), itemcont(value) {}
	explicit operator bool() const { return type != NoTarget; }
	void				clear() { type = NoTarget; creature = 0; }
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
	virtual int			getzorder() const { return 0; }// Priority for z-order sortering (lesser was be most visible). If there is two drawable in same position.
	virtual bool		hittest(point position) const { return false; }
	virtual bool		isvisible() const { return true; }
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
	void				cashing(const char* text, int width);
	virtual void		prerender();
	void				reset();
};
struct scrolllist : scrolltext {
	virtual void		row(rect rc, int n) = 0;
};
struct scrollitem : scrolllist {
	struct item*		data[10];
	int					maximum_items;
	int					mx, my;
	scrollitem(int mx, int my) : maximum_items(0), mx(mx), my(my) {}
	void				row(rect rc, int n) override {}
	item*				get(int index) const;
	int					getcount() const { return mx * my; }
	void				update(creature* player, int item_in_line);
	void				update(container* object, int item_in_line);
	void				update(short unsigned index, int item_in_line);
	void				view(creature* player, int x, int y, int dx, int dy, const rect& rcs, void(*item_proc)());
};
struct cursorset {
	cursorset(res::tokens r = res::CURSORS, int f = 267, bool single = false);
	~cursorset();
	static void			set(res::tokens rid = res::CURSORS, int frame = 267, bool single = false);
	static res::tokens	getres();
private:
	res::tokens			rid;
	int					frame;
	bool				single;
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
	constexpr coloration() : skin(0), hair(0), minor(0), major(0), armor(/*28*/MetalSteel), metal(MetalIron), leather(23) {}
	void				set(short unsigned portrait);
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
	point				getposition() const override { return{(short)(box.x1 + box.width() / 2), (short)box.y2}; }
	int					getzorder() const override { return 128; }
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
	point				getposition() const override { return *this; }
	int					getzorder() const override { return -height-256; }
	bool				is(unsigned value) const { return (flags & value) != 0; }
	virtual bool		isvisible() const override;
	void				painting(point screen) const override;
};
struct selectable : drawable {
	virtual aref<point>	getpoints() const = 0;
	point				getposition() const override;
	bool				hittest(point hittest) const;
	void				painting(point screen) const override;
};
struct item {
	explicit operator bool() const { return type != NoItem; }
	constexpr item(item_s t = NoItem) : type(t), effect(0), count(0), identified(0), magic(Mundane), quality(0), stolen(0), damaged(0) {}
	void				clear();
	int					getac() const;
	int					getarmorindex() const;
	const dice&			getattack() const;
	int					getbonus() const;
	int					getcost() const;
	int					getcount() const;
	feat_s				getfeat() const;
	static const char*	getfname(int type);
	static const char*	getfgname(int type);
	int					getframe() const;
	magic_s				getmagic() const { return magic; }
	int					getportrait() const { return type * 2; }
	int					getdragportrait() const { return type * 2 + 1; }
	item_s				gettype() const { return type; }
	bool				is(feat_s value) const;
	bool				is(slot_s value) const;
	bool				isbow() const;
	bool				isranged() const { return isbow() || isxbow() || isthrown(); }
	bool				isreach() const;
	bool				isthrown() const;
	bool				istwohand() const;
	bool				isxbow() const;
	void				setcount(int value);
private:
	item_s				type;
	unsigned char		effect;
	unsigned char		count;
	struct {
		magic_s			magic : 2;
		unsigned char	quality : 2;
		unsigned char	damaged : 2;
		unsigned char	identified : 1;
		unsigned char	stolen : 1;
	};
};
struct itemdrag {
	item*				source;
	item*				target;
	item				value;
	slot_s				target_slot;
};
struct itemground : item, drawable {
	short unsigned		index;
	virtual point		getposition() const;
	virtual rect		getrect() const override;
	virtual int			getzorder() const override;
	virtual bool		hittest(point position) const override;
	virtual void		painting(point position) const;
};
struct spell_info {
	struct duration_info {
		unsigned char	levels;
		unsigned char	rounds;
	};
	const char*			id;
	const char*			name;
	char				rsname[10];
	school_s			school;
	aset<char, LastClass+1, class_s> levels;
	duration_info		duration;
};
struct school_info {
	const char*			id;
	const char*			name;
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
	bool				is(class_s id) const;
	static aref<portrait_info> getelements();
};
struct race_info {
	const char*			id;
	const char*			name;
	char				abilities[6];
	skillset			skills;
	feat_s				feats[8];
	char				quick_learn; // Human's ability additional skills nad feats at start of game
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
struct roll_info {
	char				bonus, rolled, result, dc;
	constexpr roll_info() : bonus(0), rolled(0), result(0), dc(0) {}
	explicit operator bool() const;
	bool				iscritical(int modifier) const { return rolled >= (20 - modifier); }
};
struct attack_info : dice, roll_info {
	attack_info() = default;
	constexpr attack_info(const dice& d, char c = 0, char m = 0) : dice(d), critical(c), multiplier(m), ac(0), weapon(0) {};
	constexpr attack_info(char a) : dice(), critical(0), multiplier(0), ac(a), weapon(0) {};
	char				critical;
	char				multiplier;
	char				ac;
	item*				weapon;
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
	point				use;
	rect				box;
	char				move_to_area[8];
	char				move_to_entrance[32];
	aref<point>			points;
	const char*			getid() const override { return name; }
	rect				getrect() const override { return box; }
	int					getcursor() const override;
	aref<point>			getpoints() const override { return points; }
	virtual bool		isvisible() const { return type!=RegionTriger; }
	void				painting(point screen) const {}
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
	bool				show_search;
	bool				show_path;
};
struct creature;
struct targetreaction : target {
	void				(creature::*method)(const target& e);
	short unsigned		reach;
	constexpr targetreaction() : target(), method(0), reach(0) {}
	constexpr targetreaction(const target& e) : target(e), method(0), reach(0) {}
	void				clear();
};
struct treasure {
	int					cp, sp, gp, pp, gems, arts, mundane, minor, medium, major;
	treasure();
	void				clear();
	void				generate(int level);
	void				place();
private:
	void				place(item_s it, int count, int min, int max);
};
struct actor : drawable {
	void				act(int player, const char* format, ...);
	virtual void		blockimpassable() const {}
	void				choose_apearance(const char* title, const char* step_title);
	void				clear();
	void				clearcolors();
	static res::tokens	getanimation(item_s type);
	static res::tokens	getanimation(race_s race, gender_s gender, class_s type, int ai, int& ws);
	static point		getbackward(point start, int step, int orientation);
	static point		getcamera();
	static point		getcamerasize();
	int					getcicle() const;
	int					getciclecount(int cicle) const;
	virtual class_s		getclass() const { return Fighter; }
	int					getflags() const;
	unsigned			getfps() const override { return 12; }
	static point		getforward(point start, int step, int orientation);
	int					getframe() const { return frame; }
	virtual gender_s	getgender() const { return Male; }
	virtual int			gethits() const { return 0; }
	int					getmovedistance(point destination, short unsigned minimum_reach) const;
	virtual const char* getname() const { return ""; }
	virtual monster_s	getkind() const { return Character; }
	virtual int			getportrait() const { return 0; }
	point				getposition() const override { return position; }
	static point		getposition(point start, point dst, formation_s formation, int pos);
	virtual race_s		getrace() const { return Human; }
	rect				getrect() const override;
	virtual int			getsize() const { return 1; }
	virtual int			getspeed() const { return 9; }
	const sprite*		getsprite(int& wi) const;
	static const sprite* getsprite(res::tokens id, int wi);
	virtual const item	getwear(slot_s id) const { return NoItem; }
	virtual int			getzorder() const override;
	bool				hittest(point pt) const override;
	virtual void		interacting(const targetreaction& e) {}
	virtual bool		is(state_s id) const { return false; }
	virtual bool		isblock(point value) const { return false; }
	virtual bool		isselected() const { return false; }
	virtual bool		isstunned() const { return false; }
	bool				isvisible() const { return position.x!=0 || position.y != 0; }
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
	static short unsigned random_portrait(gender_s gender, race_s race, class_s type);
	void				render_attack(int number, point position);
	void				render_hit(bool fatal);
	void				render_path(const rect& rc, int mx, int my) const;
	void				render_marker(const rect& rc, int ox, int oy) const;
	void				say(const char* format, ...) const;
	virtual void		set(gender_s value) {}
	virtual void		set(state_s value) {}
	void				set(const targetreaction& e) { action_target = e; }
	static void			setcamera(point camera);
	void				setorientation(unsigned char value) { orientation = value; }
	void				setposition(point newpos);
	static void			slide(const point camera);
	void				stop();
	void				update() override;
	void				update_portrait();
	void				wait(char percent = 0);
private:
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
	animate_s			getattackanimate(int number) const;
	bool				isready() const;
	void				set(animate_s value);
};
struct monster_info {
	struct class_info {
		class_s			type;
		unsigned char	level;
	};
	const char*			name;
	sprite_type_s		sptype;
	res::tokens			sprites[4];
	alignment_s			alignment;
	class_info			classes[3];
	char				ability[Charisma+1];
	item_s				equipment[8];
	skillset			skills;
	cflags<feat_s>		feats;
};
struct creature : actor {
	explicit operator bool() const { return ability[0] != 0; }
	void* operator new(unsigned size);
	void operator delete (void* data);
	void				attack(creature& enemy);
	void				attack(const target& enemy);
	void				add(item e);
	void				add(stringbuilder& sb, variant v1) const;
	void				add(stringbuilder& sb, variant v1, variant v2, const char* title, bool sort_by_name = true) const;
	void				add(stringbuilder& sb, const aref<variant>& elements, const char* title) const;
	void				addinfo(stringbuilder& sb) const;
	static void			adventure_combat();
	static void			adventure();
	void				apply(race_s id, bool add_ability);
	void				apply(class_s id);
	void				apply(variant type, char level, bool interactive);
	void				blockimpassable() const override;
	void				clear();
	void				clear(variant_s value);
	void				close(const target& e);
	void				choose_action();
	bool				choose_feats(const char* title, const char* step_title, aref<variant> elements, const unsigned* minimal, char points, bool interactive);
	bool				choose_skills(const char* title, const char* step_title, aref<variant> elements, const char* minimal, char points, char points_per_skill, bool interactive);
	bool				choose_skills(const char* title, const aref<variant>& elements, bool add_ability, bool interactive);
	void				choose_skills(const char* title, const aref<variant>& elements);
	static target		choose_target(int cursor, short unsigned start, short unsigned max_cost);
	void				create(monster_s type, reaction_s reaction);
	static creature*	create(monster_s type, reaction_s reaction, point postition);
	static void			create(monster_s type, reaction_s reaction, point postition, unsigned char orientation, int count);
	void				create(class_s type, race_s race, gender_s gender, reaction_s reaction);
	static void			create_party();
	void				damage(int count);
	bool				equip(const item e);
	void				generate(const char* title);
	int					get(ability_s id) const;
	int					get(save_s id) const;
	int					get(skill_s id) const;
	int					get(class_s id) const { return classes[id]; }
	int					get(feat_s id) const { return is(id) ? 1 : 0; }
	int					get(spell_s id) const;
	item*				get(slot_s id) { return wears + id; }
	void				get(attack_info& result, slot_s slot) const;
	void				get(attack_info& result, slot_s slot, const creature& enemy) const;
	static ability_s	getability(save_s id);
	int					getac(bool flatfooted) const;
	static creature*	getactive();
	int					getbab() const;
	creature*			getbest(const aref<creature*>& source, bool (creature::*proc)(const creature& opponent) const) const;
	int					getcasterlevel() const;
	int					getcharlevel() const;
	class_s				getclass() const override;
	int					getcost(skill_s id) const { return isclass(id) ? 1 : 2; }
	static creature*	getcreature(point position);
	static creature*	getcreature(short unsigned index);
	static creature*	getcreature(aref<creature> source, short unsigned index);
	static const char*	getdescription(variant id);
	void				getdescription(stringbuilder& sb) const;
	diety_s				getdiety() const { return diety; }
	int					getfeats() const;
	gender_s			getgender() const override { return gender; }
	void				getin(const target& e);
	int					getinitiative() const;
	int					getinitiativeroll() const { return initiative; }
	int					gethits() const override { return hits; }
	int					gethitsmax() const;
	monster_s			getkind() const override { return kind; }
	int					getlevel() const;
	short unsigned		getindex() const;
	const char*			getname() const override { return name; }
	int					getmaxcarry() const;
	static int			getmoney();
	int					getmovement() const;
	const item&			getoffhand() const { return wears[QuickOffhand + active_weapon * 2]; }
	static int			getpartymaxdistance(point position);
	int					getpoints(class_s id) const;
	int					getportrait() const override { return portrait; }
	int					getprepared(variant type) const;
	int					getprepared(spell_s id, variant type) const;
	int					getr(ability_s id) const { return ability[id]; }
	race_s				getrace() const override { return race; }
	short unsigned		getreach() const;
	int					getskillpoints() const;
	int					getspellslots(variant type, int spell_level) const;
	const sprite*		getsprite(int& wi) const;
	variant_s			getstep() const;
	int					getquick() const { return active_weapon; }
	const item&			getweapon() const { return wears[QuickWeapon + active_weapon * 2]; }
	const item			getwear(slot_s id) const override;
	static void			help();
	void				icon(int x, int y, item* pi, slot_s id, itemdrag* pd);
	void				icon(int x, int y, item* pi, slot_s id, itemdrag* pd, const runable& cmd);
	void				icon(int x, int y, slot_s id, itemdrag* pd) { icon(x, y, wears + id, id, pd); }
	void				iconqw(int x, int y, int n, itemdrag* pd);
	void				interact(const targetreaction& e, short unsigned maximum_range, bool synchronized);
	void				interacting(const targetreaction& e) override;
	bool				is(feat_s id) const { return (feats[id / 32] & (1 << (id % 32))) != 0; }
	static bool			is(spell_s id, class_s cls, int level);
	bool				is(state_s id) const override { return (state&(1 << id))!=0; }
	bool				isallow(feat_s id) const;
	static bool			isallow(feat_s id, const unsigned char* ability, char character_level, char base_attack);
	bool				isallow(const item& it) const;
	bool				isallow(variant id) const;
	bool				isblock(point value) const override;
	bool				isclass(skill_s id) const;
	static bool			iscombatmode();
	bool				isenemy(const creature& opponent) const;
	bool				isranged() const { return wears[active_weapon].isranged(); }
	bool				isselected() const override;
	static bool			isgood(class_s id, save_s value);
	bool				isknown(spell_s id) const { return (spells_known[id / 32] & (1 << (id % 32)))!=0; }
	bool				isplayer() const;
	bool				isready() const { return ability[0]>0 && hits > 0; }
	void				invertory();
	void				invertory(itemdrag* pd);
	static void			journal();
	static void			makecombat();
	static void			minimap();
	static void			moveto(aref<creature> players, point pt, formation_s formation);
	static void			options();
	void				remove(feat_s id) { feats[id / 32] &= ~(1 << (id % 32)); }
	void				remove(state_s id) { state &= ~(1 << id); }
	bool				roll(roll_info& e) const;
	void				open(const target& e);
	void				say(const char* format, ...) const;
	aref<variant>		select(const aref<variant>& source, const variant v1, const variant v2, bool sort_by_name) const;
	static aref<creature*> select(const aref<creature*>& destination, const aref<creature*>& source, const creature* player, bool(creature::*proc)(const creature& e) const, short unsigned range_maximum = 0, short unsigned range_index = Blocked);
	aref<variant>		selecth(const aref<variant>& source, const variant v1, const variant v2, bool sort_by_name) const;
	void				set(ability_s id, int value) { ability[id] = value; }
	void				set(alignment_s value) { alignment = value; }
	void				set(feat_s id) { feats[id / 32] |= (1 << (id % 32)); }
	void				set(state_s id) override { state |= 1 << id; }
	void				set(gender_s value) override { gender = value; }
	void				set(coloration& value) const;
	void				set(reaction_s value) { reaction = value; }
	void				set(race_s value) { race = value; }
	void				set(skill_s id, int value) { skills[id] = value; }
	void				set(variant value);
	void				setactive();
	void				setactive(const target& e) { if(e.type==Creature) e.creature->setactive(); }
	void				setknown(spell_s id) { spells_known[id / 32] |= (1 << (id % 32)); }
	static void			setmoney(int value);
	void				setportrait(int value) { portrait = value; }
	void				setprepared(spell_s id, variant type, int count);
	void				setquick(int value) { active_weapon = value; }
	void				sheet();
	void				spellbook();
	static void			spellinfo(spell_s id);
	void				talk(const target& e) {}
	void				toggle(const target& e);
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
	monster_s			kind;
	gender_s			gender;
	race_s				race;
	alignment_s			alignment;
	diety_s				diety;
	reaction_s			reaction;
	const char*			name;
	unsigned char		ability[Charisma + 1];
	char				classes[LastClass + 1];
	char				skills[LastSkill + 1];
	unsigned			feats[LastFeat / 32 + 1];
	unsigned			state;
	short				hits, hits_rolled;
	unsigned			spells_known[LastSpell / 32 + 1];
	item				wears[LastQuickItem + 1];
	char				initiative;
	unsigned char		active_weapon;
	unsigned short		portrait;
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
void					drop(short unsigned index, item it);
entrance*				find(const char* id);
const sprite*			getareasprite();
int						getday(unsigned value);
point					getentrance(const char* name, unsigned char* orient = 0);
point					getfree(point position, int size);
unsigned short			getindex(point position);
unsigned short			getindex(point position, int size);
int						gethour(unsigned value);
const sprite*			getminimap();
unsigned char			getorientation(point s, point d);
color*					getpallette();
const char*				getpassedtime(char* result, const char* result_maximum, unsigned value);
point					getposition(short unsigned index, int size);
inline int				getrange(int feets) { return (feets / 5) * 2; }
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
int						dlgcho(const char* text, const char* a1, const char* a2);
void					dlgmsg(const char* text);
int						field(rect rc, char* result, unsigned maximum);
unsigned				getframe();
unsigned				getframe(int fps);
inline void				image(int x, int y, res::tokens token, int id, int flags = 0, unsigned char alpha = 0xFF) { draw::image(x, y, gres(token), id, flags, alpha); }
bool					inside(point t, point* points, int count);
bool					isnext(void(*proc)());
int						label(int x, int y, int width, int height, const char* name, int header = 0, int color = 0, bool border = false);
int						labell(int x, int y, int width, int height, const char* name, int header = 0, int color = 0);
int						labelr(int x, int y, int width, int height, const char* name, int header = 0, int color = 0);
void					menumodal(bool use_keys = true, itemdrag* pd = 0);
void					mslog(const char* format, ...);
void					mslogv(const char* temp);
void					mspaint(const rect& rc, const rect& rcs);
extern surface			pallette;
void					picker(int x, int y, unsigned char index, int type, const runable& cmd);
void					set(color * dest, unsigned char index, int start = 4, int count = 12);
void					textblend(point pos, const char* text, unsigned duration);
void					translate(hotkey* keys);
void					view(rect rc, rect rcs, int pixels_per_line, scrolllist& e);
void					view(rect rc, rect rcs, const char* text, scrolltext& e);
}
int								compare_variant(const void* v1, const void* v2);
template<class T> void			getrule(stringbuilder& sb, T value);
extern adat<animation, 128>		animation_data;
extern class_info				class_data[];
extern adat<creature, 256>		creature_data;
extern adat<container, 128>		container_data;
extern adat<door, 256>			door_data;
extern adat<itemcont, 2048>		itemcont_data;
extern adat<itemground, 2048>	itemground_data;
extern adat<door_tile, 1024>	door_tiles_data;
extern adat<entrance, 128>		entrance_data;
extern adat<floattext, 32>		floattext_data;
extern monster_info				monster_data[];
extern creature					players[6];
extern portrait_info			portrait_data[];
extern race_info				race_data[];
extern adat<region, 256>		region_data;
extern setting					settings;
extern school_info				school_data[];
extern skill_info				skill_data[];
extern spell_info				spell_data[];
extern adat<point, 256 * 256>	verticles;

template<> void archive::set<container>(container& e);
template<> void archive::set<door>(door& e);
template<> void archive::set<entrance>(entrance& e);
template<> void archive::set<region>(region& e);
template<> void archive::set<animation>(animation& e);