enum { // t_ms_sequence.behavior
	MS_OR,
	MS_AND,
	MS_PIPE
}

typedef struct s_ms_sequence {
	uint8_t		behavior;
	uint8_t*	is_sequence;
	void		*object;
	/*
	is_sequence == 0:
		char**
	is_sequence == 1:
		t_ms_sequence*
	*/
}	t_ms_sequence;
