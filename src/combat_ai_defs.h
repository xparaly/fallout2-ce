#ifndef COMBAT_AI_DEFS_H
#define COMBAT_AI_DEFS_H

namespace fallout {

typedef enum AreaAttackMode {
    AREA_ATTACK_MODE_ALWAYS,
    AREA_ATTACK_MODE_SOMETIMES,
    AREA_ATTACK_MODE_BE_SURE,
    AREA_ATTACK_MODE_BE_CAREFUL,
    AREA_ATTACK_MODE_BE_ABSOLUTELY_SURE,
    AREA_ATTACK_MODE_COUNT,
} AreaAttackMode;

typedef enum RunAwayMode {
    RUN_AWAY_MODE_NONE,
    RUN_AWAY_MODE_COWARD,
    RUN_AWAY_MODE_FINGER_HURTS,
    RUN_AWAY_MODE_BLEEDING,
    RUN_AWAY_MODE_NOT_FEELING_GOOD,
    RUN_AWAY_MODE_TOURNIQUET,
    RUN_AWAY_MODE_NEVER,
    RUN_AWAY_MODE_COUNT,
} RunAwayMode;

typedef enum BestWeapon {
    BEST_WEAPON_NO_PREF,
    BEST_WEAPON_MELEE,
    BEST_WEAPON_MELEE_OVER_RANGED,
    BEST_WEAPON_RANGED_OVER_MELEE,
    BEST_WEAPON_RANGED,
    BEST_WEAPON_UNARMED,
    BEST_WEAPON_UNARMED_OVER_THROW,
    BEST_WEAPON_RANDOM,
    BEST_WEAPON_COUNT,
} BestWeapon;

typedef enum DistanceMode {
    DISTANCE_STAY_CLOSE,
    DISTANCE_CHARGE,
    DISTANCE_SNIPE,
    DISTANCE_ON_YOUR_OWN,
    DISTANCE_STAY,
    DISTANCE_COUNT,
} DistanceMode;

typedef enum AttackWho {
    ATTACK_WHO_WHOMEVER_ATTACKING_ME,
    ATTACK_WHO_STRONGEST,
    ATTACK_WHO_WEAKEST,
    ATTACK_WHO_WHOMEVER,
    ATTACK_WHO_CLOSEST,
    ATTACK_WHO_COUNT,
} AttackWho;

typedef enum ChemUse {
    CHEM_USE_CLEAN,
    CHEM_USE_STIMS_WHEN_HURT_LITTLE,
    CHEM_USE_STIMS_WHEN_HURT_LOTS,
    CHEM_USE_SOMETIMES,
    CHEM_USE_ANYTIME,
    CHEM_USE_ALWAYS,
    CHEM_USE_COUNT,
} ChemUse;

typedef enum Disposition {
    DISPOSITION_NONE,
    DISPOSITION_CUSTOM,
    DISPOSITION_COWARD,
    DISPOSITION_DEFENSIVE,
    DISPOSITION_AGGRESSIVE,
    DISPOSITION_BERKSERK,
    DISPOSITION_COUNT,
} Disposition;

typedef enum HurtTooMuch {
    HURT_BLIND,
    HURT_CRIPPLED,
    HURT_CRIPPLED_LEGS,
    HURT_CRIPPLED_ARMS,
    HURT_COUNT,
} HurtTooMuch;

} // namespace fallout

#endif /* COMBAT_AI_DEFS_H */
