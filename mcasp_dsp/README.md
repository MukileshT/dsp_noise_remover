# C6748 McASP DSP audio project

This project is a bare-metal audio playback example for the Texas Instruments
TMS320C6748/LCDK. It configures the AIC31 codec over I2C, configures the
McASP peripheral for 16-bit stereo I2S, and uses EDMA to move audio samples
between memory and the McASP data register without copying every sample in an
interrupt.

The program plays the embedded audio twice:

1. The original samples from `audio_samples.h`.
2. The same samples after a 129-tap low-pass FIR filter.

The two stereo output slots receive the same mono sample, so the embedded
audio is heard on both left and right channels.

## Project files

| File | Purpose |
|---|---|
| `mcaspPlayBk_prerecorded.c` | Application entry point, McASP/EDMA setup, playback loop, and interrupt handlers. |
| `aic31.c/.h` | AIC31 codec register definitions and codec configuration helpers. |
| `codecif.c/.h` | Interrupt-driven I2C register read/write interface used by the codec driver. |
| `dsp_filter.c/.h` | Runtime generation and execution of the low-pass FIR filter. |
| `audio_samples.h` | C array containing the prerecorded PCM samples. |
| `noisy_audio.wav` | WAV source used to create sample data. |
| `wav_to_header.py` | Converts an 8-bit or 16-bit PCM WAV file into a C header. |
| `C6748.cmd` | Linker command file for the C6748 memory layout. |
| `targetConfigs/LCDKC6748.ccxml` | Code Composer Studio target configuration. |
| `ais_image.bin` | Existing binary image generated for the target boot flow. |

## Startup and data flow

`main()` performs the following sequence:

1. Enables the I2C, McASP, EDMA controller, and EDMA transfer controller
   peripheral clocks.
2. Initializes the DSP or ARM interrupt controller, depending on the build
   target.
3. Calls `I2CCodecIfInit()` to configure I2C at 100 kHz and select the AIC31
   device address `0x18`.
4. Initializes EDMA and registers the EDMA completion interrupt.
5. Registers the McASP error interrupt.
6. Requests the McASP transmit and receive EDMA channels.
7. Builds the circular EDMA parameter sets with `I2SDMAParamInit()`.
8. Resets and configures the AIC31 codec with `AIC31I2SConfigure()`.
9. Configures McASP clocks, serializers, frame sync, I2S format, FIFO, and
   time slots with `McASPI2SConfigure()`.
10. Creates the FIR coefficients with `dsp_init()` and filters the complete
    embedded clip into the `filtered[]` buffer.
11. Starts McASP and EDMA with `I2SDataTxRxActivate()`.
12. Services completed receive blocks in the main loop by filling transmit
    buffers and scheduling the next transmit EDMA parameter set.

The receive path is used as a pacing source. When EDMA finishes a receive
buffer, `rxBlocksProduced` is incremented. The main loop consumes that event,
fills the next transmit buffer, and schedules it. This keeps playback paced by
the hardware instead of by a software delay.

## Buffer and EDMA design

The application uses three receive buffers and three transmit buffers. Each
buffer represents `NUM_SAMPLES_PER_AUDIO_BUF` stereo samples. Two linked EDMA
parameter sets are used for each direction:

- Receive EDMA writes McASP data into rotating receive buffers.
- Transmit EDMA reads a prepared transmit buffer and writes to McASP.
- A small `loopBuf` is the initial transmit source while the system starts.
- Completion interrupts update the linked parameter set so the next buffer is
  ready before the current transfer ends.

The transmit buffer stores four 16-bit values per loop iteration. They are
`left sample`, `left sample`, `right sample`, `right sample`, matching the
two-slot stereo transfer format used by this example.

## Function reference

### `mcaspPlayBk_prerecorded.c`

- `main()` initializes all peripherals, precomputes filtered audio, starts the
  data path, and alternates between original and filtered playback.
- `ParamTxLoopJobSet()` creates a transmit EDMA loop parameter set that points
  back to the transmit loop job.
- `I2SDMAParamInit()` initializes the receive and transmit EDMA parameter
  sets, buffer rotation indexes, and completion state.
- `AIC31I2SConfigure()` resets the codec, selects I2S mode and slot width, and
  configures the selected sample rate.
- `McASPI2SConfigure()` configures McASP for DMA-driven 16-bit, two-slot I2S.
- `EDMA3IntSetup()` maps and enables the EDMA completion interrupt.
- `McASPErrorIntSetup()` maps and enables the McASP error interrupt.
- `I2SDataTxRxActivate()` starts clocks, enables EDMA events, activates the
  serializers, and enables McASP receive/transmit.
- `BufferTxDMAActivate()` prepares a transmit EDMA parameter set for a newly
  filled buffer and links it to the next set.
- `BufferRxDMAActivate()` prepares a receive EDMA parameter set for a rotating
  receive buffer.
- `McASPRxDMAComplHandler()` records a completed receive block and reuses the
  oldest receive buffer for the next transfer.
- `McASPTxDMAComplHandler()` restores the transmit loop job after a transmit
  completion.
- `EDMA3CCComplIsr()` clears EDMA interrupt status and dispatches receive and
  transmit completion handlers.
- `McASPErrorIsr()` clears the McASP error interrupt status. It is currently a
  placeholder for application-specific recovery logic.

### `dsp_filter.c`

- `dsp_init()` calculates a normalized 129-tap Hamming-windowed sinc low-pass
  filter, clears the delay buffer, and resets the circular-buffer position.
- `dsp_voice_filter()` inserts one normalized sample into the delay line and
  computes one filtered output using FIR symmetry. The implementation uses
  paired taps to reduce the number of multiplications and includes TI DSP
  optimization directives when compiled for C674x.

The filter constants are in `dsp_filter.h`: 16 kHz sample rate, 129 taps, and
4 kHz cutoff.

### `codecif.c`

- `I2CCodecIfInit()` disables and configures the I2C master, selects the codec
  slave address, enables I2C, and registers the I2C interrupt.
- `I2CCodecIntSetup()` maps the I2C interrupt for either the DSP interrupt
  controller or ARM interrupt controller.
- `I2CCodecSendBlocking()` starts an I2C write and waits until the ISR reports
  the STOP condition.
- `I2CCodecRcvBlocking()` starts an I2C read and waits for completion.
- `I2CCodecIsr()` services transmit-ready, receive-ready, and STOP events.
- `CodecRegWrite()` writes one codec register.
- `CodecRegRead()` selects and reads one codec register.
- `CodecRegBitSet()` reads a register, sets selected bits, and writes it back.
- `CodecRegBitClr()` reads a register, clears selected bits, and writes it back.

### `aic31.c`

- `AIC31Reset()` selects codec page zero and performs a software reset.
- `AIC31DataConfig()` selects I2S/DSP/right-justified/left-justified format,
  slot width, and data offset.
- `AIC31SampleRateConfig()` writes the AIC31 sample-rate and PLL parameters
  for supported rates from 8 kHz through 96 kHz.
- `AIC31ADCInit()` enables and routes the codec ADC inputs.
- `AIC31DACInit()` powers the DACs, routes DAC output to the codec outputs,
  and sets the output levels.

## WAV conversion

The converter uses only Python’s standard library and does not require NumPy.

```powershell
py mcasp_dsp\wav_to_header.py mcasp_dsp\noisy_audio.wav mcasp_dsp\audio_samples_generated.h --symbol audio_samples
```

Optional `--per-line N` controls how many samples are written on each C line.
The generated header contains the sample rate, channel count, sample count,
and a `static const int8_t` or `static const int16_t` array.

The generated header should match the application’s expected sample format:
the current playback code treats `audio_samples[]` as signed 16-bit PCM and
normalizes each value by `32768.0f` before filtering.

## Building and debugging

This is a Code Composer Studio project for the C6748 LCDK. Open the
`mcasp_dsp` directory as a CCS project, select the `LCDKC6748` target, build,
load, and run under the debugger. The TI SDK/header/library paths referenced
by `.cproject` must be available in CCS.

The project is hardware-dependent: a normal desktop C compiler cannot build it
without the TI CSL/board support headers and libraries. The Python converter
is independent of the target hardware.

## Important configuration values

| Setting | Current value | Meaning |
|---|---:|---|
| `WORD_SIZE` | 16 | Audio word width in bits. |
| `SLOT_SIZE` | 16 | I2S slot width in bits. |
| `SAMPLING_RATE` | 16000 | Codec and McASP sample rate. |
| `NUM_I2S_CHANNELS` | 2 | Stereo transfer. |
| `NUM_BUF` | 3 | Receive/transmit buffer count. |
| `NUM_SAMPLES_PER_AUDIO_BUF` | 2000 | Samples per DMA block. |
| `I2C_SLAVE_CODEC_AIC31` | `0x18` | AIC31 I2C address. |

