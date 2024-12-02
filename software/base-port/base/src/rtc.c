#include <base.h>
#include <dev-mmio.h>

void __timer_init() {
}

void __timer_uptime(DEV_TIMER_UPTIME_T *uptime) {
  uptime->us = ((uint64_t)inl(RTC_ADDR + 4) << 32) | inl(RTC_ADDR);
}

void __timer_rtc(DEV_TIMER_RTC_T *rtc) {
  rtc->year = inl(RTC_ADDR + 28);
  rtc->month = inl(RTC_ADDR + 24);
  rtc->day = inl(RTC_ADDR + 20);
  rtc->hour = inl(RTC_ADDR + 16);
  rtc->minute = inl(RTC_ADDR + 12);
  rtc->second = inl(RTC_ADDR + 8);
}