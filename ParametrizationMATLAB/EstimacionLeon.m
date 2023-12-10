%% Cargar datos desde el archivo
datos = load('datos.txt');
dt = 0.005;
% Extraer columnas
t = (0:dt:(size(datos)-1)*dt)';
u = [t,datos(:, 1).*(12/1024)];
im = [t, datos(:, 2)];
vel = [t, datos(:, 3)];

%% Graficar los datos
figure;

subplot(3, 1, 1);
plot(t,u(:,2), 'b', 'LineWidth', 1.25);
title('Duty Cycle (Entrada)');
grid on;

subplot(3, 1, 2);
plot(t,im(:,2), 'r', 'LineWidth', 0.1);
title('Corriente Medida');
grid on;

subplot(3, 1, 3);
plot(t,vel(:,2), 'm', 'LineWidth', 0.1);
title('Velocidad Medida');
grid on;

sgtitle('Gráficos de Datos');
%% Estimacion
estado = [im(:,2)';vel(:,2)'];
y = estado(:,2:end)';
U = [estado(:,1:end-1); u(1:end-1,2)']';
theta =(pinv(U)*y)'; 
% matrices en tiempo discreto
A = theta(:,1:end-1);
B = (theta(:,end));

%matriz en tiempo continuo
Ac = logm(A*dt);
% modelo del motor de CD
 