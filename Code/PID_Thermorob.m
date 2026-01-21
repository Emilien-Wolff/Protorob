%% =================================================
%  INITIALISATION
%% =================================================
close all;
clear;
clc;

%% =================================================
%  PARAMETRES DU MCC
%% =================================================
L  = 0.5;          % Inductance [H]
R  = 3.4;          % Resistance [Ohm]
J  = 0.0007;       % Inertie [kg.m^2]
F  = 0.007;        % Frottement visqueux [N.m.s]
K_t = 0.3129;      % Constante de couple [N.m/A]
K_e = 0.3129;      % Constante FEM [V/(rad/s)]

%% =================================================
%  GAINS DU SCHEMA-BLOCS
%% =================================================
K_temp  = 2.67;        % K1  (deg/°C)
K_adapt = 1;           % K2
K_red   = 1;           % Reducteur

%% =================================================
%  CODEUR
%% =================================================
K_codeur = 180/pi;     % conversion rad -> deg (CONTINU)
Delta_theta = 360/48;  % resolution du codeur (deg)

%% =================================================
%  PWM
%% =================================================
Umax = 24;     % tension max [V]
Umin = -24;

%% =================================================
%  VARIABLE DE LAPLACE
%% =================================================
s = tf('s');

%% =================================================
%  MODELE DU MOTEUR (POSITION)
%  θ_mot(s) / U(s)
%% =================================================
G_mot = K_t / ( s*((L*s + R)*(J*s + F) + K_t*K_e) );

%% =================================================
%  PLANTE VUE PAR LE CORRECTEUR
%  θ_mes(s) / U(s)
%% =================================================
G_eq = K_adapt * G_mot * K_codeur;

%% =================================================
%  REGLAGE AUTOMATIQUE DU PI
%% =================================================
C = pidtune(G_eq,'PI');

Kp = C.Kp
Ki = C.Ki
Kd = C.Kd     % = 0

%% =================================================
%  BOUCLE FERMEE CONTINUE (IDÉALE)
%% =================================================
sys_cl = feedback(C * G_eq, 1);

%% =================================================
%  SIMULATION TEMPORELLE REALISTE
%% =================================================
t = 0:0.001:5;           % temps
theta_ref = 30*ones(size(t));   % consigne [deg]

% Sortie ideale continue
theta_cont = lsim(sys_cl, theta_ref, t);

% Quantification codeur
theta_quant = Delta_theta * round(theta_cont / Delta_theta);

%% =================================================
%  AFFICHAGE
%% =================================================
figure;
plot(t, theta_ref,'g'); hold on;
plot(t, theta_cont, 'b', 'LineWidth', 1.5); hold on;
stairs(t, theta_quant, 'r', 'LineWidth', 1.2);
grid on;
xlabel('Temps [s]');
ylabel('Position [deg]');
title('Asservissement de position – effet du codeur');
legend('Consigne','Position continue','Position quantifiée');

%% =================================================
%  ANALYSE FREQUENTIELLE
%% =================================================
figure;
margin(C * G_eq)
grid on;
title('Marges de stabilité');
