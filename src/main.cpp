/*
 *--------------------------------------
 * Program Name: RightHandRuleSolver
 * Author: CheeseBear99
 * License: GNU General Public License v3.0
 * Description: Solves E&M magnetic force equations, for any variable, and provides the direction.
 *--------------------------------------
*/

#include <ti/screen.h>
#include <ti/getcsc.h>
#include <ti/ui.h>
#include <cstring>
#include <cstdlib>
#include <mathutils.h>
#include <vec3.h>
#include <physics.h>

#define MAX_INPUT_LENGTH 20
#define NUM_FIELDS 9
#define VISIBLE 8

const char *fields_label_particle[NUM_FIELDS] =
{
    "Fb: ", "Fb_dir: ", "q: ", "v: ", "v_dir: ", "B: ", "B_dir: ", "\x5B: ", "SOLVE"
};

const char *fields_label_magnetism[NUM_FIELDS] =
{
    "Fb: ", "Fb_dir: ", "I: ", "L: ", "L_dir: ", "B: ", "B_dir: ", "\x5B: ", "SOLVE"
};

char bufs[NUM_FIELDS][MAX_INPUT_LENGTH];
uint8_t length_of_list[NUM_FIELDS];

char g_Fb[MAX_INPUT_LENGTH];
char g_Fb_dir[MAX_INPUT_LENGTH];
char g_q_or_I[MAX_INPUT_LENGTH];
char g_v_or_l[MAX_INPUT_LENGTH];
char g_v_or_l_dir[MAX_INPUT_LENGTH];
char g_B[MAX_INPUT_LENGTH];
char g_B_dir[MAX_INPUT_LENGTH];
char g_theta[MAX_INPUT_LENGTH];


void drawFields(uint8_t active_field, uint8_t scroll_top, bool is_particle);

void drawMainMenu(int8_t selected);

void printInfo();

void runParticleInput(bool is_particle);

void solve(bool is_particle);


int main()
{
    os_SetCursorPos(0, 0);

    int8_t selected = 0;
    int8_t last_selected = -1;

    while (true)
    {
        if (selected != last_selected)
        {
            drawMainMenu(selected);
            last_selected = selected;
        }

        const uint8_t key = os_GetCSC();
        if (key == sk_Up && selected > 0) selected--;
        if (key == sk_Down && selected < 2) selected++;
        if (key == sk_Enter) break;
        if (key == sk_Mode) return 0;
    }

    os_ClrHome();

    if (selected == 0)
    {
        runParticleInput(true);
        os_ClrHome();
        solve(true);
    } else if (selected == 1)
    {
        runParticleInput(false);
        os_ClrHome();
        solve(false);
    } else if (selected == 2)
    {
        printInfo();
    }

    while (true)
    {
        const uint8_t key = os_GetCSC();
        if (key == sk_Mode) return 0;
        if (key == sk_Clear) return 0;
    }
}


void solve(const bool is_particle)
{
    double Fb = std::abs(atof(g_Fb));
    double q_or_I = atof(g_q_or_I);
    double v_or_l = std::abs(atof(g_v_or_l));
    double B = std::abs(atof(g_B));
    double theta = atof(g_theta);

    double Fb_dir[3];
    double v_or_l_dir[3];
    double B_dir[3];

    getDirectionFromString(g_v_or_l_dir, v_or_l_dir);
    getDirectionFromString(g_B_dir, B_dir);
    getDirectionFromString(g_Fb_dir, Fb_dir);

    if (Fb == 0 || directionIsAllZeros(Fb_dir))
    {
        Fb = FbMagnitudeParticleOrCurrent(q_or_I, v_or_l, B, theta);
        crossProduct(v_or_l_dir, B_dir, Fb_dir);

        if (q_or_I < 0)
            multiplyVectorBy(-1, Fb_dir);

        char out_str[50];
        sprintfScientificNotation(out_str, Fb);
        os_PutStrFull("Fb = ");
        os_PutStrFull(out_str);
        os_PutStrFull(" N");
        os_NewLine();
        os_PutStrFull("Direction = ");
        os_PutStrFull(vectorToString(Fb_dir));
    } else if (q_or_I == 0)
    {
        multiplyVectorBy(Fb, Fb_dir);
        multiplyVectorBy(v_or_l, v_or_l_dir);
        multiplyVectorBy(B, B_dir);

        q_or_I = solveForqOrI(Fb_dir, v_or_l_dir, B_dir);

        if (is_particle)
        {
            char out_str[50];
            sprintfScientificNotation(out_str, q_or_I);
            os_PutStrFull("q = ");
            os_PutStrFull(out_str);
            os_PutStrFull(" C");
        } else
        {
            char out_str[50];
            sprintfScientificNotation(out_str, q_or_I);
            os_PutStrFull("I = ");
            os_PutStrFull(out_str);
            os_PutStrFull(" Amps");
        }
    } else if (v_or_l == 0 || directionIsAllZeros(v_or_l_dir))
    {
        multiplyVectorBy(Fb, Fb_dir);
        multiplyVectorBy(B, B_dir);
        multiplyVectorBy(1 / q_or_I, Fb_dir);

        reverseCrossProduct(B_dir, Fb_dir, false, v_or_l_dir);

        if (is_particle)
        {
            char out_str[50];
            sprintfScientificNotation(out_str, std::abs(getNonZeroVectorComponent(v_or_l_dir)));
            os_PutStrFull("v = ");
            os_PutStrFull(out_str);
            os_PutStrFull(" m/s");
            os_NewLine();
            os_PutStrFull("Direction = ");
            multiplyVectorBy(1 / std::abs(getNonZeroVectorComponent(v_or_l_dir)), v_or_l_dir);
            os_PutStrFull(vectorToString(v_or_l_dir));
        } else
        {
            char out_str[50];
            sprintfScientificNotation(out_str, std::abs(getNonZeroVectorComponent(v_or_l_dir)));
            os_PutStrFull("l = ");
            os_PutStrFull(out_str);
            os_PutStrFull(" m");
            os_NewLine();
            os_PutStrFull("Direction = ");
            multiplyVectorBy(1 / std::abs(getNonZeroVectorComponent(v_or_l_dir)), v_or_l_dir);
            os_PutStrFull(vectorToString(v_or_l_dir));
        }
    } else if (B == 0 || directionIsAllZeros(B_dir))
    {
        multiplyVectorBy(Fb, Fb_dir);
        multiplyVectorBy(v_or_l, v_or_l_dir);
        multiplyVectorBy(1 / q_or_I, Fb_dir);

        reverseCrossProduct(v_or_l_dir, Fb_dir, true, B_dir);

        char out_str[50];
        sprintfScientificNotation(out_str, std::abs(getNonZeroVectorComponent(B_dir)));
        os_PutStrFull("B = ");
        os_PutStrFull(out_str);
        os_PutStrFull(" T");
        os_NewLine();
        os_PutStrFull("Direction = ");
        multiplyVectorBy(1 / std::abs(getNonZeroVectorComponent(B_dir)), B_dir);
        os_PutStrFull(vectorToString(B_dir));
    } else if (theta == 0)
    {
        theta = solveForThetaD(Fb, q_or_I, v_or_l, B);

        char out_str[50];
        os_PutStrFull("\x5B = ");
        __builtin_sprintf(out_str, "%.2f.", theta);
        os_PutStrFull(out_str);
        os_PutStrFull("\x14");
    }
}

void printInfo()
{
    os_SetCursorPos(0, 0);
    os_PutStrFull("INFO");
    os_NewLine();
    os_NewLine();
    os_PutStrFull("1. Cardinal directions are");
    os_NewLine();
    os_PutStrFull("   on the XY plane, where");
    os_NewLine();
    os_PutStrFull("   north is +Y axis");
    os_NewLine();
    os_NewLine();
    os_PutStrFull("2. +Z/-Z are out-of and");
    os_NewLine();
    os_PutStrFull("   into the page respectively");
    os_NewLine();
    os_PutStrFull("3. ASK CB99 FOR HELP");
}

void drawFields(const uint8_t active_field, const uint8_t scroll_top, const bool is_particle)
{
    for (uint8_t slot = 0; slot < VISIBLE; slot++)
    {
        const uint8_t f = scroll_top + slot;

        os_SetCursorPos(slot, 0);
        os_PutStrFull("                              ");

        if (f >= NUM_FIELDS) continue;

        os_SetCursorPos(slot, 0);
        os_PutStrFull(f == active_field ? ">" : " ");

        if (is_particle)
            os_PutStrFull(fields_label_particle[f]);
        else
            os_PutStrFull(fields_label_magnetism[f]);

        os_PutStrFull(bufs[f]);
        os_PutStrFull(f == active_field ? "_" : " ");
        os_PutStrFull("          ");
    }
}

void drawMainMenu(const int8_t selected)
{
    os_ClrHome();
    for (int8_t i = 0; i < 3; i++)
    {
        const char *options[] = {"Particle", "Current In Wire", "Info"};
        os_PutStrFull(i == selected ? "> " : "  ");
        os_PutStrFull(options[i]);
        os_NewLine();
    }
}

void runParticleInput(const bool is_particle)
{
    memset(bufs, 0, sizeof(bufs));
    memset(length_of_list, 0, sizeof(length_of_list));

    uint8_t active_field_variable = 0;
    uint8_t scroll_top = 0;

    drawFields(active_field_variable, scroll_top, is_particle);

    while (true)
    {
        const uint8_t key = os_GetCSC();
        if (key == 0) continue;

        if (key == sk_Mode) return;

        if (key == sk_Up && active_field_variable > 0)
        {
            active_field_variable--;
            if (active_field_variable < scroll_top) scroll_top = active_field_variable;
            drawFields(active_field_variable, scroll_top, is_particle);
            continue;
        }

        if (key == sk_Down && active_field_variable < NUM_FIELDS - 1)
        {
            active_field_variable++;
            if (active_field_variable >= scroll_top + VISIBLE) scroll_top = active_field_variable - VISIBLE + 1;
            drawFields(active_field_variable, scroll_top, is_particle);
            continue;
        }

        if (key == sk_Enter && active_field_variable == NUM_FIELDS - 1)
        {
            os_ClrHome();

            if (key == sk_Enter && active_field_variable == NUM_FIELDS - 1)
            {
                strcpy(g_Fb, bufs[0]);
                strcpy(g_Fb_dir, bufs[1]);
                strcpy(g_q_or_I, bufs[2]);
                strcpy(g_v_or_l, bufs[3]);
                strcpy(g_v_or_l_dir, bufs[4]);
                strcpy(g_B, bufs[5]);
                strcpy(g_B_dir, bufs[6]);
                strcpy(g_theta, bufs[7]);

                os_ClrHome();
                break;
            }
            break;
        }

        if (key == sk_Enter && active_field_variable < NUM_FIELDS - 1)
        {
            active_field_variable++;
            if (active_field_variable >= scroll_top + VISIBLE) scroll_top = active_field_variable - VISIBLE + 1;
            drawFields(active_field_variable, scroll_top, is_particle);
            continue;
        }

        if (key == sk_Del && length_of_list[active_field_variable] > 0)
        {
            bufs[active_field_variable][--length_of_list[active_field_variable]] = '\0';
            drawFields(active_field_variable, scroll_top, is_particle);
            continue;
        }

        if (length_of_list[active_field_variable] >= MAX_INPUT_LENGTH - 1) continue;

        static bool is_alpha_toggled = false;

        if (key == sk_Alpha)
        {
            is_alpha_toggled = !is_alpha_toggled;
            continue;
        }

        if (key == sk_Clear)
        {
            bufs[active_field_variable][0] = '\0';
            length_of_list[active_field_variable] = 0;
            drawFields(active_field_variable, scroll_top, is_particle);
            continue;
        }

        char c = 0;
        if (!is_alpha_toggled)
        {
            if (key == sk_0) c = '0';
            else if (key == sk_1) c = '1';
            else if (key == sk_2) c = '2';
            else if (key == sk_3) c = '3';
            else if (key == sk_4) c = '4';
            else if (key == sk_5) c = '5';
            else if (key == sk_6) c = '6';
            else if (key == sk_7) c = '7';
            else if (key == sk_8) c = '8';
            else if (key == sk_9) c = '9';
            else if (key == sk_DecPnt) c = '.';
            else if (key == sk_Chs) c = '-';
            else if (key == sk_Add) c = '+';
        } else
        {
            if (key == sk_1) c = 'y';
            else if (key == sk_2) c = 'z';
            else if (key == sk_Store) c = 'x';
            else if (key == sk_Sin) c = 'E';
            is_alpha_toggled = false;
        }

        if (c)
        {
            bufs[active_field_variable][length_of_list[active_field_variable]++] = c;
            bufs[active_field_variable][length_of_list[active_field_variable]] = '\0';
            drawFields(active_field_variable, scroll_top, is_particle);
        }
    }
}
