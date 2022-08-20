#ifndef CHESSBOARD_COLORS_CPP
#define CHESSBOARD_COLORS_CPP

#include "util.cpp"
#include "typedefs.h"

namespace chessboard {

	static constexpr colordef_t hexColorToDemical(colordef_t hex) {
		return ((hex >> 16) & 0xFF) * 1000 / 256 * 1'000'000 + ((hex >> 8) & 0xFF) * 1000 / 256 * 1'000 + (hex & 0xFF) * 1000 / 256;
	}


	static constexpr colordef_t
		ON_FRAME_COLOR  = hexColorToDemical(0x2CA107),
		OFF_FRAME_COLOR = hexColorToDemical(0xD4AA00);

	static constexpr id_t
		ON_FRAME_COLOR_ID  = 0x20,
		OFF_FRAME_COLOR_ID = 0x40;

	static constexpr pair_t
		ON_FRAME_COLOR_PAIR  = COLOR_PAIR(ON_FRAME_COLOR_ID),
		OFF_FRAME_COLOR_PAIR = COLOR_PAIR(OFF_FRAME_COLOR_ID);

	static constexpr const id_t FRAME_COLOR_IDS[]    { ON_FRAME_COLOR_ID, OFF_FRAME_COLOR_ID };
	static constexpr const colordef_t FRAME_COLORS[] { ON_FRAME_COLOR,    OFF_FRAME_COLOR };


	static constexpr colordef_t
		TEXT_COLOR          = 1000 << 16 | 200,
		SELECTED_TEXT_COLOR = 1000 << 16 | 500,
		BORDER_COLOR        = 800,
		WHITE_FIELD_COLOR   = 1000,
		BLACK_FIELD_COLOR   = 000,

		WHITE_CHECKER_COLOR = 750,
		BLACK_CHECKER_COLOR = 150;

	static constexpr pair_t
		TEXT_COLOR_PAIR          = COLOR_PAIR(1),
		SELECTED_TEXT_COLOR_PAIR = COLOR_PAIR(2),
		BORDER_COLOR_PAIR        = COLOR_PAIR(3),
		WHITE_FIELD_COLOR_PAIR   = COLOR_PAIR(4),
		BLACK_FIELD_COLOR_PAIR   = COLOR_PAIR(5);

	static constexpr const pair_t CHECKER_COLOR_PAIRS[] {
		COLOR_PAIR(6),
		COLOR_PAIR(7),
		COLOR_PAIR(8),
		COLOR_PAIR(9),
	};

	static constexpr const colordef_t COLOR_VALUES[] {
		TEXT_COLOR,
		SELECTED_TEXT_COLOR,
		BORDER_COLOR,
		WHITE_FIELD_COLOR,
		BLACK_FIELD_COLOR,

		WHITE_CHECKER_COLOR << 16 | WHITE_FIELD_COLOR,
		BLACK_CHECKER_COLOR << 16 | WHITE_FIELD_COLOR,
		WHITE_CHECKER_COLOR << 16 | BLACK_FIELD_COLOR,
		BLACK_CHECKER_COLOR << 16 | BLACK_FIELD_COLOR,
	};


	static void init_color_pairs() {

		for(unsigned int i = 0; i < size(FRAME_COLOR_IDS); ++i) {
			const id_t id = FRAME_COLOR_IDS[i];
			const colordef_t color = FRAME_COLORS[i];
			init_color(id, static_cast<color_t>(color / 1000'000), static_cast<color_t>(color / 1000 % 1000), static_cast<color_t>(color % 1000));
			init_pair(id, 0, id);
		}

		for(id_t id = 0; id < static_cast<id_t>(size(COLOR_VALUES)); ) {
			const colordef_t color = COLOR_VALUES[id++];
			const color_t
					background = static_cast<color_t>(color),
					foreground = static_cast<color_t>(color >> 16);

			init_color(id, background, background, background);

			id_t foregroundID;

			if(foreground != 0) {
				foregroundID = static_cast<id_t>(0x10 + id);
				init_color(foregroundID, foreground, foreground, foreground);
			} else {
				foregroundID = 0;
			}

			init_pair(id, foregroundID, id);
			for(unsigned int i = 0; i < size(FRAME_COLOR_IDS); ++i) {
				id_t frameColorID = FRAME_COLOR_IDS[i];
				init_pair(static_cast<id_t>(id + frameColorID), frameColorID, id);
				if(foregroundID != 0)
					init_pair(static_cast<id_t>(foregroundID + frameColorID), frameColorID, foregroundID);
			}
		}
	}
}

#endif
