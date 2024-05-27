#include<stdio.h>// 1-1~2-4 완료
#include<Windows.h>
#include<stdlib.h>
#include<time.h>
#define LEN_MIN 15
#define LEN_MAX 50
#define STM_MIN 0
#define STM_MAX 5
#define PROB_MIN 10
#define PROB_MAX 90
#define AGGRO_MIN 0
#define AGGRO_MAX 5
#define MOVE_LEFT 1
#define MOVE_STAY 0
#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2
#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

int citizen_location, madong_location, zombie_location;
int zombie_switch = 0, citizen_switch = 0;
int train_len = -1, madong_stm = -1, prob = -1;
int citizen_aggro = 1, madong_aggro = 1;

void intro();
void input();
int move_prob(int);
void show_current_train_condition();
void show_move_statement(int);
void madong_move();
void z_c_action();
void madong_action();
int decide_outro();
void happy_outro();
void sad_outro_c_d();
void sad_outro_m_d();

int main()
{
	srand((unsigned int)time(NULL));
	int time = 2;

	intro();
	input();

	madong_location = train_len - 2;
	zombie_location = train_len - 3;
	citizen_location = train_len - 6;

	printf("\n초기 열차 상태\n");
	show_current_train_condition();
	printf("\n\n\n\n");

	while (1) {
		printf("\n===========================================\n      %d번째 턴 <이동> 페이즈 \n===========================================\n", time - 1);
		move_prob(time);
		show_current_train_condition();
		show_move_statement(time);
		printf("\n===========================================\n      %d번째 턴 <행동> 페이즈 \n===========================================\n", time - 1);
		z_c_action();
		madong_action();
		time++;
		Sleep(400);
	}
	return 0;
}


void input() {
	while (1) {
		if (train_len < LEN_MIN || train_len > LEN_MAX) {
			printf("train length(%d~%d)>>", LEN_MIN, LEN_MAX);
			scanf_s("%d", &train_len);
			continue;
		}

		if (madong_stm < STM_MIN || madong_stm > STM_MAX) {
			printf("madongseok stamina(%d~%d)>>", STM_MIN, STM_MAX);
			scanf_s("%d", &madong_stm);
			continue;
		}
		if (prob < PROB_MIN || prob > PROB_MAX) {
			printf("percentile probability 'p'(%d~%d)>>", PROB_MIN, PROB_MAX);
			scanf_s("%d", &prob);
			continue;
		}
		break;
	}
}

int move_prob(int time) {
	if (prob <= rand() % 100 + 1)
	{
		citizen_location--;
		citizen_switch = 1;
		if (citizen_aggro < AGGRO_MAX) {
			citizen_aggro++;
		}
	}
	else {
		if (citizen_aggro > AGGRO_MIN) {
			citizen_aggro--;
		}
	}
	if (zombie_switch != 3) {
		if (prob >= rand() % 100 + 1 && time % 2 == 0)
		{
			if (citizen_aggro > madong_aggro || citizen_aggro == madong_aggro) {
				zombie_location--;
				zombie_switch = 1;
			}
			else if (citizen_aggro < madong_aggro) {
				if (!(zombie_location + 1 == madong_location)) {
					zombie_location++;
					zombie_switch = 2;
				}
			}

		}
	}
}

void show_current_train_condition() {
	int count = 0;
	while (count < train_len) {
		printf("#");
		count++;
	}
	count = 1;
	printf("\n#");
	while (!(count == train_len - 1)) {
		if (count == citizen_location) {
			printf("C");
		}
		if (count == zombie_location) {
			printf("Z");
		}
		if (count == madong_location) {
			printf("M");
		}
		if (!(count == citizen_location) && !(count == zombie_location) && !(count == madong_location)) {
			printf(" ");
		}
		count++;
	}
	count = 0;
	printf("#\n");
	while (count < train_len) {
		printf("#");
		count++;
	}
	printf("\n");
}
void show_move_statement(int time) {
	if (citizen_switch == 1) {
		printf("\ncitizen: %d -> %d (aggro: %d)\n", citizen_location + 1, citizen_location, citizen_aggro);
		citizen_switch = 0;
	}
	else if (citizen_switch == 0) {
		printf("\ncitizen : stay %d (aggro: %d)\n", citizen_location, citizen_aggro);
	}
	if (zombie_switch == 0) {
		if (time % 2 == 1) {
			printf("zombie : stay %d (cannot move)\n\n", zombie_location);
		}
		else
			printf("zombie : stay %d\n\n", zombie_location);
	}
	if (zombie_switch == 1) {
		printf("zombie: %d -> %d\n\n", zombie_location + 1, zombie_location);
		zombie_switch = 0;
	}
	if (zombie_switch == 2) {
		printf("zombie: %d -> %d\n\n", zombie_location - 1, zombie_location);
		zombie_switch = 0;
	}
	if (zombie_switch == 3) {
		printf("좀비는 마동석에게 잡혀 이동이 불가합니다.\n");
		zombie_switch = 0;
	}
	madong_move();
	printf("\n\n\n\n");
}
void madong_move() {
	int madong_move_switch = -1;
	while (1) {
		if (madong_location == zombie_location + 1) {
			printf("madongseok move(0:stay)>> ");
			scanf_s("%d", &madong_move_switch);
			if (madong_move_switch == 0) {
				if (madong_aggro > AGGRO_MIN) {
					madong_aggro--;
				}
				show_current_train_condition();
				printf("madongseok: stay %d(aggro: %d, stamina: %d)", madong_location, madong_aggro, madong_stm);
				break;
			}
			else {
				continue;
			}

		}
		else {
			printf("madongseok move(0:stay, 1:left)>>");
			scanf_s("%d", &madong_move_switch);
			if (madong_move_switch == 0 || madong_move_switch == 1) {
				if (madong_move_switch == 0) {
					if (madong_aggro > AGGRO_MIN) {
						madong_aggro--;
					}
					show_current_train_condition();
					printf("\nmadongseok: stay %d(aggro: %d, stamina: %d)", madong_location, madong_aggro, madong_stm);
				}
				else {
					if (madong_aggro < AGGRO_MAX) {
						madong_aggro++;
					}
					madong_location--;
					show_current_train_condition();
					printf("\nmadongseok: %d -> %d (aggro: %d, stamina: %d)", madong_location + 1, madong_location, madong_aggro, madong_stm);
				}
				break;
			}
			else {
				continue;
			}
		}
	}
}
void z_c_action() {
	printf("\n\ncitizen does nothing.\n");
	if ((zombie_location - 1 == citizen_location) && (zombie_location + 1 == madong_location)) {
		if (madong_aggro > citizen_aggro) {
			printf("Zombie attacked madongseok (aggro: %d vs. %d, madongseok stamina: %d -> %d)\n\n", citizen_aggro, madong_aggro, madong_stm, madong_stm - 1);
			madong_stm--;
			decide_outro;
		}
		else if (madong_aggro < citizen_aggro) {
			printf("GAME OVER! citizen dead...\n");
			decide_outro();
		}
	}
	if (zombie_location - 1 == citizen_location) {
		printf("GAME OEVER! citizen dead...\n");
		decide_outro();
	}
	if (zombie_location + 1 == madong_location) {
		printf("Zombie attacked madongseok\n");
		madong_stm--;
		decide_outro();
	}
	if (!(zombie_location - 1 == citizen_location) && !(zombie_location + 1 == madong_location)) {
		printf("zombie attacked nobady.\n");
	}
}
void madong_action() {
	int choice_madong_action = -1;
	while (1) {
		if (madong_location == zombie_location + 1) {
			printf("madongseok action(0.rest, 1.provoke, 2.pull)>> ");
			scanf_s("%d", &choice_madong_action);
			if (choice_madong_action == 0 || choice_madong_action == 1 || choice_madong_action == 2) {
				break;
			}
			continue;
		}
		else {
			printf("madongseok action(0.rest, 1.provoke)>> ");
			scanf_s("%d", &choice_madong_action);
			if (choice_madong_action == 0 || choice_madong_action == 1) {
				break;
			}
			continue;

		}
	}
	switch (choice_madong_action) {
	case 0:
		printf("madongseok rests...\n");
		if (madong_aggro == AGGRO_MIN && madong_stm == STM_MAX) {
			printf("madongseok: %d (aggro: %d, stamina: %d)\n", madong_location, madong_aggro, madong_stm);
		}
		if (madong_aggro == AGGRO_MIN && madong_stm != STM_MAX) {
			printf("madongseok: %d (aggro: %d, stamina: %d -> %d)\n", madong_location, madong_aggro, madong_stm, madong_stm + 1);
			madong_stm++;
		}
		if (madong_aggro != AGGRO_MIN && madong_stm == STM_MAX) {
			printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)\n", madong_location, madong_aggro, madong_aggro - 1, madong_stm, madong_stm + 1);
			madong_aggro--;
		}
		if (madong_aggro != AGGRO_MIN && madong_stm != STM_MAX) {
			printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)\n", madong_location, madong_aggro, madong_aggro - 1, madong_stm, madong_stm + 1);
			madong_aggro--;
			madong_stm++;
		}
		break;
	case 1:
		printf("madongseok provoked zombie...\n");
		printf("madongseok: %d (aggro: %d -> %d, stamina: %d)\n", madong_location, madong_aggro, AGGRO_MAX, madong_stm);
		madong_aggro = AGGRO_MAX;
		break;
	case 2:
		if (prob <= rand() % 100 + 1) {
			if (AGGRO_MAX - madong_aggro <= 2) {
				printf("madongseok pulled zombie... Next turn, it can't move\n");
				printf("madongseok: %d (aggro: %d, stamina: %d -> %d)\n", madong_location, AGGRO_MAX, madong_stm, madong_stm - 1);
				madong_aggro = AGGRO_MAX;
				madong_stm--;
				zombie_switch = 3;
				decide_outro();
			}
			else {
				printf("madongseok pulled zombie... Next turn, it can't move\n");
				printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)\n", madong_location, madong_aggro, madong_aggro + 2, madong_stm, madong_stm - 1);
				madong_aggro += 2;
				madong_stm--;
				zombie_switch = 3;
				decide_outro();
			}
		}
		else {
			printf("madongseok failed to pull zombie\n");
			printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)\n", madong_location, madong_aggro, madong_aggro + 2, madong_stm, madong_stm - 1);
			madong_aggro += 2;
			madong_stm--;
			decide_outro();
		}
		break;
	}
}
int decide_outro() {
	if (citizen_location == 1) {
		happy_outro();
		exit(0);
	}
	if (citizen_location + 1 == zombie_location) {
		sad_outro_c_d();
		exit(0);
	}
	if (madong_stm <= STM_MIN) {
		sad_outro_m_d();
		exit(0);
	}
}

void intro() {
	int count = 0;
	printf("\n\n");
	printf("#######  #######  ##   ##  #######  ######   #######           #######  ##   ##  ###  ##   ##   ##   \n");
	printf("   ###   ##   ##  ### ###  #    ##    ##     ##                #    ##  ##   ##  #### ##   ##   ##   \n");
	printf("  ###    ##   ##  #######  ######     ##     ######            #######  ##   ##  ## ####   ##   ##   \n");
	printf(" ###     ##   ##  ## # ##  ##   ##    ##     ##                ##  ##   ##   ##  ##  ###             \n");
	printf("#######  #######  ##   ##  #######  ######   #######           ##   ##  #######  ##   ##   ##   ## (this is art.......)   \n");
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	printf("                        #     ####  ###   #  ##   #   ###  \n");
	printf("                        #     #  #  #  #  #  # #  #  #  # #\n");
	printf("                        #     #  #  #  #  #  #  # #  #    #\n");
	printf("                        ####  ####  ###   #  #   ##   #####  #  #  #  # \n\n");
	while (count < 15) {
		printf("#######");
		Sleep(70);
		count++;
	}
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	printf("                  ######  ########     ##      #######  ########  ##  ##\n");
	printf("                 ##          ##       ####     #    ##     ##     ##  ##\n");
	printf("                   ###       ##      ##  ##    #######     ##     ##  ##\n");
	printf("                     ##      ##      ######    ##  ##      ##           \n");
	printf("                 #####       ##     ##    ##   ##   ##     ##     ##  ##\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("게임 설명\n");
	printf("좀비를 피해 다음 열차로 이동해 주세요!!\n");
	printf("train length의 길이가 길어질수록 플레이 타임이 길어지고...\n");
	printf("p의 값이 클수록 난이도가 올라갑니다......\n");
	printf("값 입력 후 당신이 할 수 있는 건 오직 기도뿐... 무운을 빕니다...\n");
	printf("===================================================================================================================\n");
	printf("\n\n\n\n\n\n\n\n\n\n\n");
}

void happy_outro() {
	printf("\n\n\n");
	printf("==================================================================================================================");
	printf("\n\n\n\n");
	printf("##    ##  #######   ##   ##        ######  ##   ##   #######  ##      ##  ##  ##      ##  #######   ######  \n");
	printf(" ##  ##   ##   ##   ##   ##       ##       ##   ##   #    ##   ##    ##   ##   ##    ##   ##        ##    ##\n");
	printf("  ####    ##   ##   ##   ##        ####    ##   ##   #######    ##  ##    ##    ##  ##    #######   ##    ##\n");
	printf("   ##     ##   ##   ##   ##           ##   ##   ##   ##  ##      ####            ####     ##        ##    ##\n");
	printf("   ##     #######   #######       ######   #######   ##   ##      ##      ##      ##      #######   ######  \n");
	printf("\n\n시민은 무사히 다음 열차로 이동했습니다.\n\n");
}
void sad_outro_c_d() {
	printf("\n\n\n");
	printf("==================================================================================================================");
	printf("\n\n\n\n");
	printf("##    ##  #######   ##   ##       ######    ##  #######   ######  \n");
	printf(" ##  ##   ##   ##   ##   ##       ##    ##  ##  ##        ##    ##\n");
	printf("  ####    ##   ##   ##   ##       ##    ##  ##  #######   ##    ##\n");
	printf("   ##     ##   ##   ##   ##       ##    ##  ##  ##        ##    ##\n");
	printf("   ##     #######   #######       ######    ##  #######   ######  \n");
	printf("\n\n시민은 좀비에게 잡혔습니다......\n\n");
}
void sad_outro_m_d() {
	printf("\n\n\n");
	printf("==================================================================================================================");
	printf("\n\n\n\n");
	printf("##    ##  #######   ##   ##       ######    ##  #######   ######  \n");
	printf(" ##  ##   ##   ##   ##   ##       ##    ##  ##  ##        ##    ##\n");
	printf("  ####    ##   ##   ##   ##       ##    ##  ##  #######   ##    ##\n");
	printf("   ##     ##   ##   ##   ##       ##    ##  ##  ##        ##    ##\n");
	printf("   ##     #######   #######       ######    ##  #######   ######  \n");
	printf("\n\n마동석이 죽었습니다......\n\n");
}