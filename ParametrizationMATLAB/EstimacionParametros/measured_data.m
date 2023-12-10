% Cargar datos desde el archivo
clc
datos = load('datos.txt');

% Extraer columnas
t = (0:0.001:(4922)*0.001)';
dc = [t,datos(:, 1)];  %Ciclo de trabajo V de la fuente 12 V
q = [t, datos(:, 2)];  %Posicion angular 
im = [t, datos(:, 3) * (3.3 / 4095)];   %Im corriente motor

% Graficar los datos
% figure;
% subplot(3, 1, 1);
% plot(dc(:,2));
% title('Duty Cycle (Entrada)');
% subplot(3, 1, 2);
% plot(q(:,2));
% title('Posición Angular Medida');
% subplot(3, 1, 3);
% plot(im(:,2));
% title('Corriente Medida');
% sgtitle('Gráficos de Datos');

u=12*datos(:, 1)';
pos=datos(:, 2);
im=datos(:, 3) * (3.3 / 4095);
velocidad=(datos(2:end,2)-datos(1:end-1,2))/(1e-3);
estado=[im(1:end-1)';velocidad'];
y=estado(:,2:end)';
Ur=[estado(:,1:end-1);u(1:end-2)]';
theta=pinv(Ur)*y;
theta=theta';
A=theta(:,1:end-1);
B=theta(:,end);
%%
subplot(2,1,1)
plot(out.Salida.Time,out.Salida.Data(:,2))
subplot(2,1,2)
plot(im)
