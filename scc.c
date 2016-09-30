int get_base_spell_complexity_char(CHAR_DATA * ch) {
    OBJ_DATA *obj;
	float complexity = 0, total_complexity = 0, complexity_modifier;
	int level = 0, classes = 0;
	register int i;

	level = ch->level;

	if (IS_IMMORTAL(ch)) { //Imms
debug_msg("scc", "%s(%d): %s: returning 100", __func__, __LINE__, name(ch));
		return 100;
	}
	if (IS_NPC(ch)) { // Mobs
		complexity = 5.0 + (0.375 * get_level(ch));
		complexity = UMIN(complexity, 100.0);
debug_msg("scc", "%s(%d): %s: returning %d", __func__, __LINE__, name(ch), (int) complexity);
		return (int) complexity;
	}
	
	//Players
	classes = (59 + ch->pcdata->total_level) / 60;
	
	for (i = 0; i < MAX_CLASS_HISTORY; i++) {
		if (ch->pcdata->class_history[i] < 0) {
debug_msg("scc", "%s(%d): %s: class %d doesn't exist", __func__, __LINE__, name(ch), i);
			continue;
		}
		complexity = UMIN(90, level);
		switch (ch->pcdata->class_history[i]) {
            default:
                complexity *= 0;
			case CLASS_WIZARD:
				complexity *= 1.28;
				complexity += get_stat_average(ch, STAT_KNOWLEDGE, 0, 0, 0, 0);
				break;
			case CLASS_PRIEST:
				complexity *= 1.25;
				complexity += get_stat_average(ch, STAT_WISDOM, 0, 0, 0, 0);
				break;
			case CLASS_WITCH:
				complexity *= 1.25;
				complexity += get_stat_average(ch, STAT_WISDOM, 0, 0, 0, 0);
				break;
			case CLASS_SHAMAN:
				complexity *= 1.25;
				complexity += get_stat_average(ch, STAT_WISDOM, 0, 0, 0, 0);
				break;
			case CLASS_DRUID:
				complexity *= 1.25;
				complexity += get_stat_average(ch, STAT_KNOWLEDGE, STAT_WISDOM, 0, 0, 0);
				break;
			case CLASS_BARD:
				complexity *= 0.60;
				complexity += get_stat_average(ch, STAT_PERSONALITY, STAT_LUCK, 0, 0, 0);
				break;
			case CLASS_RANGER:
				complexity *= 0.96;
				complexity += get_stat_average(ch, STAT_KNOWLEDGE, 0, 0, 0, 0);
				break;
			case CLASS_MONK:
				complexity *= 1.30;
				complexity += get_stat_average(ch, STAT_KNOWLEDGE, 0, 0, 0, 0);
				break;
			case CLASS_PALADIN:
				complexity *= 0.70;
				complexity += get_stat_average(ch, STAT_WISDOM, 0, 0, 0, 0);
				break;
			case CLASS_PSIONIC:
				complexity *= 1.25;
				complexity += get_stat_average(ch, STAT_KNOWLEDGE, STAT_SANITY, STAT_VITALITY, STAT_WISDOM, 0);
				break;
			case CLASS_ARCHON:
				complexity += get_stat_average(ch, STAT_PERSONALITY, 0, 0, 0, 0);
				break;
		}
debug_msg("scc", "%s(%d): %s: class %d is %d: total %d += %d complexity -> %d",
                  __func__,
                     __LINE__, 
                          name(ch), 
                                    i,
                                          ch->pcdata->class_history[i],
                                                    (int) total_complexity,
                                                          (int) complexity,
                                                                           (int) (total_complexity + complexity)
);
		total_complexity += complexity;
	}
debug_msg("scc", "%s(%d): %d total / %d classes = %d", __func__, __LINE__, (int) total_complexity, classes, (int) total_complexity / classes);
	total_complexity /= classes;

	if ((complexity_modifier = get_special_stat(ch, STAT_SCC)) != 0) {
debug_msg("scc", "%s(%d): +%d scc stat", __func__, __LINE__, (int) complexity_modifier);
		total_complexity += complexity_modifier;
	}
	//SML 10/18/10 Gives a minimum of +5 SCC per class, to lessen the penalty for multiclassing.
	total_complexity += 4 * classes;
debug_msg("scc", "%s(%d): +4 * %d classes = %d", __func__, __LINE__, classes, (int) total_complexity);
	
    if (IS_ARCHON(ch)) {
        total_complexity += (ch->level - 60) / 5;
    }

    total_complexity += get_amulet_values(ch, AMULET_SCC);
    
	total_complexity = URANGE(0, total_complexity, 100);
		
	return (int) total_complexity;
}

int get_stat_average(CHAR_DATA *ch, int stat1, int stat2, int stat3, int stat4, int stat5) {
	int i = 0, total = 0;
	
	if (stat1 > 0) {
		total += get_curr_stat(ch, stat1);
		i++;
	}
	if (stat2 > 0) {
		total += get_curr_stat(ch, stat2);
		i++;
	}
	if (stat3 > 0) {
		total += get_curr_stat(ch, stat3);
		i++;
	}
	if (stat4 > 0) {
		total += get_curr_stat(ch, stat4);
		i++;
	}
	if (stat5 > 0) {
		total += get_curr_stat(ch, stat5);
		i++;
	}
	
	total /= i;
    return total;
}
