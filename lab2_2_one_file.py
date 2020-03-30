#Системи реального часу. Лабораторна робота 2.2. Швидке перетворення Фур'є
#Кравчука Олександра ІВ-72
import numpy as np
import matplotlib.pyplot as plt
import random as rnd

def generate_signal(n:int, Wmax:float, N:int, step:float = 1.0):
    Wmax2 = Wmax*2*np.pi #циклічна частота
    A, fi, w = 0.0, 0.0, 0.0
    maxArgument = step * (N-1)
    signal = np.zeros(N, dtype=np.float32)
    t = np.linspace(0, maxArgument, N, dtype=np.float32)
    for i in range(n):
        A = rnd.random()*0.5+0.5 #амплітуда від 0.5 до 1.0
        fi = rnd.uniform(0, 2 * np.pi)
        w = Wmax2 / n * (i + 1)

        signal += A * np.sin(w * t + fi)
    return t, signal

def fast_fourier_transform(a:np.ndarray):
    n=a.size
    if n==1: return
    n_2=n//2
    #a0, a1 - парні, непарні елементи масиву a відповідно
    a0 = np.empty(n_2, dtype=np.complex)
    a1 = np.empty(n_2, dtype=np.complex)
    j=0
    for i in range(0,n,2): #розділення
        a0[j]=a[i]
        a1[j]=a[i+1]
        j+=1
    #рекурсія
    fast_fourier_transform(a0)
    fast_fourier_transform(a1)

    ang = 2*np.pi/n
    w = 1.0+0.0j #WpN
    wn = np.cos(ang)+np.sin(ang)*1.0j #W1N

    for i  in range(0,n_2): #об'єднання
        a[i]=a0[i] + w * a1[i] #операція "метелик"
        a[i + n_2] = a0[i] - w * a1[i]
        w *= wn

if __name__=="__main__":
    n = 8  # n-число гармонік
    Wmax = 1200  # Wmax-гранична частота
    N = 8192  # N-кількість дискретних відліків
    N_2 = N//2+1
    step = 0.0004  # step - крок аргументу для дискретних відліків

    t, signal1 = generate_signal(n, Wmax, N, step)
    T = t[N - 1]  # період часу

    signalComplex = np.array(signal1, dtype=np.complex)
    fast_fourier_transform(signalComplex)
    amplitudeSpectrum = np.abs(signalComplex[:N_2])

    freq = np.linspace(1 / T, 0.5 / step, amplitudeSpectrum.size)

    #######################################################
    # print("T = ", T, "(0.5/step) = ", 0.5/step)
    # print("fmin = ", 1/T, " fmax = ", 0.5 / step)

    fig1, (ax1, ax2) = plt.subplots(2)
    fig1.suptitle("Згенерований сигнал\nРезультат швилкого перетворення Фур'є")

    ax1.set_ylabel('x(t) - сигнал')
    ax1.set_xlabel('t')

    ax1.plot(t, signal1, color='blue', linewidth=0.5)
    ax1.grid(True)

    ax2.set_xlabel('частота  (f)')
    ax2.set_ylabel('амплітуда')
    ax2.plot(freq, amplitudeSpectrum, color='green', linewidth=1)
    ax2.grid(True)

    plt.show()
